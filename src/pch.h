#pragma once

#define _USE_MATH_DEFINES

#include <SDL3/SDL.h>
#include <algorithm>
#include <array>
#include <boost/algorithm/string.hpp>
#include <boost/di.hpp>
#include <boost/di/extension/injections/lazy.hpp>
#include <boost/di/extension/scopes/shared.hpp>
#include <boost/serialization/strong_typedef.hpp>
#include <cerrno>
#include <climits>
#include <cmath>
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <filesystem>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fstream>
#include <functional>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_internal.h>
#include <iostream>
#include <magic_enum/magic_enum.hpp>
#include <map>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <numbers>
#include <optional>
#include <random>
#include <regex>
#include <set>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <boost/di.hpp>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
