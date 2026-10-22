#pragma once

#include <string>
#include <utility>
#include <vector>

#include "common/types.hpp"

namespace industrial_camera {

struct YamlNode {
  enum class Kind { kEmpty, kScalar, kMap, kSeq };
  Kind kind{Kind::kEmpty};
  std::string scalar;
  std::vector<std::pair<std::string, YamlNode>> entries;
  std::vector<YamlNode> seq;

  const YamlNode* Find(const std::string& key) const;
  std::string AsString(const std::string& fallback = "") const;
  int AsInt(int fallback) const;
  double AsDouble(double fallback) const;
  bool AsBool(bool fallback) const;
};

struct LoadResult {
  bool ok{false};
  ProjectConfig config;
  std::string error;
};

YamlNode ParseYaml(const std::string& text, std::string* error);
LoadResult LoadProjectConfig(const std::string& topology_path);

}  // namespace industrial_camera
