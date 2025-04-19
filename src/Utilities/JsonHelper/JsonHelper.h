#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace glm
{
  // Serialization: glm::ivec2 -> JSON
  inline void
  to_json(nlohmann::json& json, const glm::ivec2& vector)
  {
    json = { { "x", vector.x }, { "y", vector.y } };
  }

  inline void
  from_json(const nlohmann::json& json, glm::ivec2& vector)
  {
    vector.x = json.value("x", 0);
    vector.y = json.value("y", 0);
  }

  // Serialization: glm::ivec3 -> JSON
  inline void
  to_json(nlohmann::json& json, const glm::ivec3& vector)
  {
    json = { { "x", vector.x }, { "y", vector.y }, { "z", vector.z } };
  }

  inline void
  from_json(const nlohmann::json& json, glm::ivec3& vector)
  {
    vector.x = json.value("x", 0);
    vector.y = json.value("y", 0);
    vector.z = json.value("z", 0);
  }

  // Serialization: glm::ivec4 -> JSON
  inline void
  to_json(nlohmann::json& json, const glm::ivec4& vector)
  {
    json = {
      { "x", vector.x }, { "y", vector.y }, { "z", vector.z }, { "w", vector.w }
    };
  }

  inline void
  from_json(const nlohmann::json& json, glm::ivec4& vector)
  {
    vector.x = json.value("x", 0);
    vector.y = json.value("y", 0);
    vector.z = json.value("z", 0);
    vector.w = json.value("w", 0);
  }

  // Serialization: glm::vec3 -> JSON
  inline void
  to_json(nlohmann::json& json, const glm::vec2& vector)
  {
    json = { { "x", vector.x }, { "y", vector.y } };
  }

  inline void
  from_json(const nlohmann::json& json, glm::vec2& vector)
  {
    vector.x = json.value("x", 0.0F);
    vector.y = json.value("y", 0.0F);
  }

  // Serialization: glm::vec3 -> JSON
  inline void
  to_json(nlohmann::json& json, const glm::vec3& vector)
  {
    json = { { "x", vector.x }, { "y", vector.y }, { "z", vector.z } };
  }

  inline void
  from_json(const nlohmann::json& json, glm::vec3& vector)
  {
    vector.x = json.value("x", 0.0F);
    vector.y = json.value("y", 0.0F);
    vector.z = json.value("z", 0.0F);
  }

  // Serialization: glm::vec3 -> JSON
  inline void
  to_json(nlohmann::json& json, const glm::vec4& vector)
  {
    json = {
      { "x", vector.x }, { "y", vector.y }, { "z", vector.z }, { "w", vector.w }
    };
  }

  inline void
  from_json(const nlohmann::json& json, glm::vec4& vector)
  {
    vector.x = json.value("x", 0.0F);
    vector.y = json.value("y", 0.0F);
    vector.z = json.value("z", 0.0F);
    vector.w = json.value("w", 0.0F);
  }

  // Serialization: glm::vec3 -> JSON
  inline void
  to_json(nlohmann::json& json, const glm::uvec2& vector)
  {
    json = { { "x", vector.x }, { "y", vector.y } };
  }

  inline void
  from_json(const nlohmann::json& json, glm::uvec2& vector)
  {
    vector.x = json.value("x", 0U);
    vector.y = json.value("y", 0U);
  }

  // Serialization: glm::vec3 -> JSON
  inline void
  to_json(nlohmann::json& json, const glm::uvec3& vector)
  {
    json = { { "x", vector.x }, { "y", vector.y }, { "z", vector.z } };
  }

  inline void
  from_json(const nlohmann::json& json, glm::uvec3& vector)
  {
    vector.x = json.value("x", 0U);
    vector.y = json.value("y", 0U);
    vector.z = json.value("z", 0U);
  }

  // Serialization: glm::vec3 -> JSON
  inline void
  to_json(nlohmann::json& json, const glm::uvec4& vector)
  {
    json = {
      { "x", vector.x }, { "y", vector.y }, { "z", vector.z }, { "w", vector.w }
    };
  }

  inline void
  from_json(const nlohmann::json& json, glm::uvec4& vector)
  {
    vector.x = json.value("x", 0U);
    vector.y = json.value("y", 0U);
    vector.z = json.value("z", 0U);
    vector.w = json.value("w", 0U);
  }
}