#pragma once

#define GLM_ENABLE_EXPERIMENTAL

// System headers
#include <cerrno>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>

// STL and C++
#include <algorithm>
#include <array>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <numbers>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

// Third party
#include <SDL3/SDL.h>
#include <boost/algorithm/string.hpp>
#include <boost/di.hpp>
#include <boost/di/extension/injections/lazy.hpp>
#include <boost/di/extension/scopes/shared.hpp>
#include <boost/serialization/strong_typedef.hpp>
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <glm/common.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
