#pragma once

#include <string>
#include <vector>

#include "common/types.hpp"

namespace industrial_camera {

struct ValidationResult {
  bool ok{true};
  std::vector<std::string> errors;
};

ValidationResult ValidateProject(const ProjectConfig& config);

}  // namespace industrial_camera
