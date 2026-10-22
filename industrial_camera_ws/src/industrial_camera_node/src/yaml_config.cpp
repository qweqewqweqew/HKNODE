#include "common/yaml_config.hpp"

#include <cctype>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace industrial_camera {
namespace {

std::string Trim(std::string text) {
  while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front()))) {
    text.erase(text.begin());
  }
  while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back()))) {
    text.pop_back();
  }
  return text;
}

std::string StripComment(const std::string& text) {
  bool quoted = false;
  for (std::size_t i = 0; i < text.size(); ++i) {
    if (text[i] == '"' && (i == 0 || text[i - 1] != '\\')) {
      quoted = !quoted;
    }
    if (text[i] == '#' && !quoted) {
      return text.substr(0, i);
    }
  }
  return text;
}

std::string Unquote(std::string text) {
  text = Trim(text);
  if (text.size() >= 2 && text.front() == '"' && text.back() == '"') {
    std::string out;
    for (std::size_t i = 1; i + 1 < text.size(); ++i) {
      if (text[i] == '\\' && i + 1 + 1 <= text.size()) {
        out.push_back(text[i + 1]);
        ++i;
      } else {
        out.push_back(text[i]);
      }
    }
    return out;
  }
  return text;
}

struct RawLine {
  int indent{0};
  bool is_seq{false};
  std::string content;
};

std::vector<RawLine> SplitLines(const std::string& text, std::string* error) {
  std::vector<RawLine> lines;
  std::istringstream input(text);
  std::string raw;
  int number = 0;
  while (std::getline(input, raw)) {
    ++number;
    if (!raw.empty() && raw.back() == '\r') {
      raw.pop_back();
    }
    int indent = 0;
    while (indent < static_cast<int>(raw.size()) && raw[indent] == ' ') {
      ++indent;
    }
    if (indent < static_cast<int>(raw.size()) && raw[indent] == '\t') {
      if (error != nullptr) {
        *error = "YAML 不支持 Tab 缩进，行 " + std::to_string(number);
      }
      return {};
    }
    std::string body = StripComment(raw.substr(static_cast<std::size_t>(indent)));
    body = Trim(body);
    if (body.empty()) {
      continue;
    }
    RawLine line;
    line.indent = indent;
    if (body.rfind("- ", 0) == 0 || body == "-") {
      line.is_seq = true;
      line.content = Trim(body == "-" ? "" : body.substr(2));
    } else {
      line.content = body;
    }
    lines.push_back(std::move(line));
  }
  return lines;
}

bool SplitKey(const std::string& text, std::string* key, bool* has_value, std::string* value) {
  bool quoted = false;
  for (std::size_t i = 0; i < text.size(); ++i) {
    if (text[i] == '"' && (i == 0 || text[i - 1] != '\\')) {
      quoted = !quoted;
    }
    if (text[i] == ':' && !quoted) {
      *key = Trim(text.substr(0, i));
      *value = Trim(text.substr(i + 1));
      *has_value = !value->empty();
      return !key->empty();
    }
  }
  return false;
}

void FillFlowSeq(const std::string& text, YamlNode* node) {
  node->kind = YamlNode::Kind::kSeq;
  std::string body = text.substr(1, text.size() - 2);
  std::string item;
  bool quoted = false;
  auto push = [&]() {
    std::string piece = Unquote(Trim(item));
    if (!piece.empty()) {
      YamlNode child;
      child.kind = YamlNode::Kind::kScalar;
      child.scalar = piece;
      node->seq.push_back(std::move(child));
    }
    item.clear();
  };
  for (char ch : body) {
    if (ch == '"') {
      quoted = !quoted;
    }
    if (ch == ',' && !quoted) {
      push();
    } else {
      item.push_back(ch);
    }
  }
  push();
}

std::size_t ParseBlock(const std::vector<RawLine>& lines, std::size_t index, YamlNode* node,
                       std::string* error);

std::size_t ParseMap(const std::vector<RawLine>& lines, std::size_t index, int indent,
                     YamlNode* node, std::string* error) {
  node->kind = YamlNode::Kind::kMap;
  while (index < lines.size() && lines[index].indent == indent && !lines[index].is_seq) {
    std::string key;
    std::string value;
    bool has_value = false;
    if (!SplitKey(lines[index].content, &key, &has_value, &value)) {
      if (error != nullptr) {
        *error = "无法解析 YAML 键: " + lines[index].content;
      }
      return lines.size();
    }
    ++index;
    YamlNode child;
    if (!has_value) {
      if (index < lines.size() && lines[index].indent > indent) {
        index = ParseBlock(lines, index, &child, error);
      } else {
        child.kind = YamlNode::Kind::kScalar;
      }
    } else if (value.size() >= 2 && value.front() == '[' && value.back() == ']') {
      FillFlowSeq(value, &child);
    } else {
      child.kind = YamlNode::Kind::kScalar;
      child.scalar = Unquote(value);
    }
    node->entries.emplace_back(key, std::move(child));
  }
  return index;
}

std::size_t ParseSeq(const std::vector<RawLine>& lines, std::size_t index, int indent,
                     YamlNode* node, std::string* error) {
  node->kind = YamlNode::Kind::kSeq;
  while (index < lines.size() && lines[index].indent == indent && lines[index].is_seq) {
    const std::string rest = lines[index].content;
    ++index;
    YamlNode item;
    std::string key;
    std::string value;
    bool has_value = false;
    if (rest.empty()) {
      if (index < lines.size() && lines[index].indent > indent) {
        index = ParseBlock(lines, index, &item, error);
      }
    } else if (SplitKey(rest, &key, &has_value, &value)) {
      item.kind = YamlNode::Kind::kMap;
      YamlNode first;
      if (has_value) {
        if (value.size() >= 2 && value.front() == '[' && value.back() == ']') {
          FillFlowSeq(value, &first);
        } else {
          first.kind = YamlNode::Kind::kScalar;
          first.scalar = Unquote(value);
        }
      }
      item.entries.emplace_back(key, std::move(first));
      while (index < lines.size() && lines[index].indent > indent && !lines[index].is_seq) {
        std::string child_key;
        std::string child_value;
        bool child_has = false;
        if (!SplitKey(lines[index].content, &child_key, &child_has, &child_value)) {
          if (error != nullptr) {
            *error = "序列元素中的键无法解析: " + lines[index].content;
          }
          return lines.size();
        }
        ++index;
        YamlNode child;
        if (!child_has && index < lines.size() && lines[index].indent > indent) {
          index = ParseBlock(lines, index, &child, error);
        } else {
          child.kind = YamlNode::Kind::kScalar;
          child.scalar = Unquote(child_value);
        }
        item.entries.emplace_back(child_key, std::move(child));
      }
    } else {
      item.kind = YamlNode::Kind::kScalar;
      item.scalar = Unquote(rest);
    }
    node->seq.push_back(std::move(item));
  }
  return index;
}

std::size_t ParseBlock(const std::vector<RawLine>& lines, std::size_t index, YamlNode* node,
                       std::string* error) {
  if (index >= lines.size()) {
    node->kind = YamlNode::Kind::kEmpty;
    return index;
  }
  if (lines[index].is_seq) {
    return ParseSeq(lines, index, lines[index].indent, node, error);
  }
  return ParseMap(lines, index, lines[index].indent, node, error);
}

bool ReadText(const std::filesystem::path& path, std::string* text, std::string* error) {
  std::ifstream input(path, std::ios::binary);
  if (!input) {
    if (error != nullptr) {
      *error = "无法读取文件: " + path.string();
    }
    return false;
  }
  std::ostringstream buffer;
  buffer << input.rdbuf();
  *text = buffer.str();
  return true;
}

std::filesystem::path ResolveFeature(const std::filesystem::path& topology,
                                     const std::string& feature_file, std::string* error) {
  const std::filesystem::path feature(feature_file);
  std::vector<std::filesystem::path> candidates;
  if (feature.is_absolute()) {
    candidates.push_back(feature);
  }
  candidates.push_back(std::filesystem::current_path() / feature);
  candidates.push_back(topology.parent_path().parent_path() / feature);
  candidates.push_back(topology.parent_path() / feature);
  for (const auto& candidate : candidates) {
    if (std::filesystem::exists(candidate)) {
      return candidate;
    }
  }
  if (error != nullptr) {
    *error = "找不到第二层配置: " + feature_file;
  }
  return {};
}

bool LoadFeatures(const std::filesystem::path& path, std::vector<FeatureEntry>* features,
                  std::string* error) {
  std::string text;
  if (!ReadText(path, &text, error)) {
    return false;
  }
  YamlNode root = ParseYaml(text, error);
  if (!error->empty()) {
    return false;
  }
  const YamlNode* list = root.Find("features");
  if (list == nullptr || list->kind != YamlNode::Kind::kSeq) {
    if (error != nullptr) {
      *error = path.string() + " 缺少 features 列表";
    }
    return false;
  }
  for (const auto& item : list->seq) {
    const YamlNode* name = item.Find("feature");
    const YamlNode* type = item.Find("type");
    const YamlNode* value = item.Find("value");
    if (name == nullptr || type == nullptr || value == nullptr) {
      if (error != nullptr) {
        *error = path.string() + " 的属性项必须包含 feature、type、value";
      }
      return false;
    }
    FeatureEntry entry;
    entry.feature = name->AsString();
    entry.value = value->AsString();
    if (!ParseParamType(type->AsString(), &entry.type)) {
      if (error != nullptr) {
        *error = "属性 type 不在白名单: " + type->AsString();
      }
      return false;
    }
    if (entry.feature.empty()) {
      if (error != nullptr) {
        *error = "属性名为空";
      }
      return false;
    }
    features->push_back(std::move(entry));
  }
  return true;
}

}  // namespace

const YamlNode* YamlNode::Find(const std::string& key) const {
  for (const auto& entry : entries) {
    if (entry.first == key) {
      return &entry.second;
    }
  }
  return nullptr;
}

std::string YamlNode::AsString(const std::string& fallback) const {
  return kind == Kind::kScalar ? scalar : fallback;
}

int YamlNode::AsInt(int fallback) const {
  if (kind != Kind::kScalar || scalar.empty()) {
    return fallback;
  }
  try {
    return std::stoi(scalar);
  } catch (const std::exception&) {
    return fallback;
  }
}

double YamlNode::AsDouble(double fallback) const {
  if (kind != Kind::kScalar || scalar.empty()) {
    return fallback;
  }
  try {
    return std::stod(scalar);
  } catch (const std::exception&) {
    return fallback;
  }
}

bool YamlNode::AsBool(bool fallback) const {
  if (kind != Kind::kScalar) {
    return fallback;
  }
  if (scalar == "true" || scalar == "True" || scalar == "1") {
    return true;
  }
  if (scalar == "false" || scalar == "False" || scalar == "0") {
    return false;
  }
  return fallback;
}

YamlNode ParseYaml(const std::string& text, std::string* error) {
  std::string local_error;
  std::string* err = error == nullptr ? &local_error : error;
  err->clear();
  const std::vector<RawLine> lines = SplitLines(text, err);
  if (!err->empty()) {
    return {};
  }
  YamlNode root;
  if (lines.empty()) {
    root.kind = YamlNode::Kind::kEmpty;
    return root;
  }
  ParseBlock(lines, 0, &root, err);
  return root;
}

LoadResult LoadProjectConfig(const std::string& topology_path) {
  LoadResult result;
  const std::filesystem::path path(topology_path);
  std::string text;
  if (!ReadText(path, &text, &result.error)) {
    return result;
  }
  YamlNode root = ParseYaml(text, &result.error);
  if (!result.error.empty()) {
    return result;
  }
  const YamlNode* manager = root.Find("camera_manager_node");
  const YamlNode* params = manager == nullptr ? &root : manager->Find("ros__parameters");
  if (params == nullptr) {
    params = manager;
  }
  if (params == nullptr) {
    result.error = "拓扑文件缺少 camera_manager_node.ros__parameters";
    return result;
  }
  result.config.project_name = params->Find("project_name") == nullptr
                                   ? result.config.project_name
                                   : params->Find("project_name")->AsString(result.config.project_name);
  if (const YamlNode* node = params->Find("log_dir")) {
    result.config.log_dir = node->AsString(result.config.log_dir);
  }
  if (const YamlNode* node = params->Find("log_level")) {
    result.config.log_level = node->AsString(result.config.log_level);
  }
  if (const YamlNode* node = params->Find("stat_report_interval_sec")) {
    result.config.stat_report_interval_sec = node->AsInt(result.config.stat_report_interval_sec);
  }
  if (const YamlNode* node = params->Find("trigger_cmd_topic")) {
    result.config.trigger_cmd_topic = node->AsString(result.config.trigger_cmd_topic);
  }
  if (const YamlNode* node = params->Find("algo_tune_topic")) {
    result.config.algo_tune_topic = node->AsString(result.config.algo_tune_topic);
  }
  if (const YamlNode* node = params->Find("set_param_service")) {
    result.config.set_param_service = node->AsString(result.config.set_param_service);
  }
  const YamlNode* active = params->Find("active_stations");
  if (active == nullptr || active->kind != YamlNode::Kind::kSeq || active->seq.empty()) {
    result.error = "active_stations 必须是非空列表";
    return result;
  }
  for (const auto& name_node : active->seq) {
    const std::string name = name_node.AsString();
    const YamlNode* station_node = params->Find(name);
    if (station_node == nullptr) {
      result.error = "active_stations 引用了不存在的工位: " + name;
      return result;
    }
    StationConfig station;
    station.name = name;
    auto take_s = [&](const char* key, std::string* dest) {
      if (const YamlNode* node = station_node->Find(key)) {
        *dest = node->AsString(*dest);
      }
    };
    auto take_i = [&](const char* key, int* dest) {
      if (const YamlNode* node = station_node->Find(key)) {
        *dest = node->AsInt(*dest);
      }
    };
    auto take_d = [&](const char* key, double* dest) {
      if (const YamlNode* node = station_node->Find(key)) {
        *dest = node->AsDouble(*dest);
      }
    };
    take_s("brand", &station.brand);
    take_s("connect_type", &station.connect_type);
    take_s("device_identity", &station.device_identity);
    take_s("optical_frame_id", &station.optical_frame_id);
    take_s("working_mode", &station.working_mode);
    take_s("trigger_source", &station.trigger_source);
    take_i("trigger_timeout_ms", &station.trigger_timeout_ms);
    take_s("topic_name", &station.topic_name);
    take_s("shm_name", &station.shm_name);
    take_s("shm_permissions", &station.shm_permissions);
    take_i("shm_slot_count", &station.shm_slot_count);
    take_i("shm_slot_capacity_mb", &station.shm_slot_capacity_mb);
    take_i("shm_lock_timeout_ms", &station.shm_lock_timeout_ms);
    take_i("reconnect_interval_ms", &station.reconnect_interval_ms);
    if (const YamlNode* node = station_node->Find("auto_tune_enable")) {
      station.auto_tune_enable = node->AsBool(station.auto_tune_enable);
    }
    take_d("auto_tune_exposure_min_us", &station.auto_tune_exposure_min_us);
    take_d("auto_tune_exposure_max_us", &station.auto_tune_exposure_max_us);
    take_d("auto_tune_gain_min_db", &station.auto_tune_gain_min_db);
    take_d("auto_tune_gain_max_db", &station.auto_tune_gain_max_db);
    take_s("feature_config_file", &station.feature_config_file);
    take_i("param_settle_us", &station.param_settle_us);
    take_i("trigger_queue_depth", &station.trigger_queue_depth);
    if (station.optical_frame_id.empty()) {
      station.optical_frame_id = station.name + "_optical_frame";
    }
    station.resolved_feature_path =
        ResolveFeature(path, station.feature_config_file, &result.error).string();
    if (!result.error.empty()) {
      return result;
    }
    if (!LoadFeatures(station.resolved_feature_path, &station.features, &result.error)) {
      return result;
    }
    station.stat_report_interval_sec = result.config.stat_report_interval_sec;
    result.config.stations.push_back(std::move(station));
  }
  result.ok = true;
  return result;
}

}  // namespace industrial_camera
