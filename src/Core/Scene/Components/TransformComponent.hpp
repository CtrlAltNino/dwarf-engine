#pragma once

#include "Utilities/ISerializable.hpp"
#include "Utilities/JsonHelper/JsonHelper.hpp"
#include <entt/entt.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <string>


namespace Dwarf
{
  /// @brief A component holding a transform.
  struct TransformComponent : public ISerializable
  {
#define RAD_2_DEG (180.0f / std::numbers::pi_v<float>)
#define DEG_2_RAD (std::numbers::pi_v<float> / 180.0f)

    /// @brief Position of the entity.
    glm::vec3 Position = { 0.0F, 0.0F, 0.0F };

    /// @brief Rotation of the entity as euler angles.
    glm::vec3 Rotation = { 0.0F, 0.0F, 0.0F };

    /// @brief Scale of the entity.
    glm::vec3 Scale = { 1.0F, 1.0F, 1.0F };

    glm::mat4 CachedMatrix = glm::mat4(1.0F);

    /// @brief Entity handle of the hierarchical parent entity.
    entt::entity Parent = entt::null;

    /// @brief List of entity handles that are child entities.
    std::vector<entt::entity> Children;

    bool DirtyFlag = true;

    TransformComponent() = default;

    TransformComponent(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
      : Position(pos)
      , Rotation(rot)
      , Scale(scale)
    {
    }

    TransformComponent(const nlohmann::json& json)
    {
      Position = json["Position"].get<glm::vec3>();
      Rotation = json["Rotation"].get<glm::vec3>();
      Scale = json["Scale"].get<glm::vec3>();
    }

    static auto
    WrapEuler(const glm::vec3& angles) -> glm::vec3
    {
      glm::vec3 wrapped = glm::mod(angles, 360.0F);
      return { wrapped.x < 0 ? wrapped.x + 360.0F : wrapped.x,
               wrapped.y < 0 ? wrapped.y + 360.0F : wrapped.y,
               wrapped.z < 0 ? wrapped.z + 360.0F : wrapped.z };
    }

    void
    UpdateMatrix()
    {
      glm::mat4 rot = glm::yawPitchRoll(glm::radians(Rotation.y),
                                        glm::radians(Rotation.x),
                                        glm::radians(Rotation.z));

      CachedMatrix = glm::translate(glm::mat4(1.0F), Position) * rot *
                     glm::scale(glm::mat4(1.0F), Scale);

      DirtyFlag = false;
    }

    // ========== Getters ==========

    [[nodiscard]] auto
    GetPosition() const -> glm::vec3
    {
      return Position;
    }

    void
    SetPosition(glm::vec3 position)
    {
      Position = position;
      DirtyFlag = true;
    }

    [[nodiscard]] auto
    GetEulerAngles() const -> glm::vec3
    {
      return Rotation;
    }

    void
    SetEulerAngles(glm::vec3 rotation)
    {
      Rotation = WrapEuler(rotation);
      DirtyFlag = true;
    }

    [[nodiscard]] auto
    GetScale() const -> glm::vec3
    {
      return Scale;
    }

    void
    SetScale(glm::vec3 scale)
    {
      Scale = scale;
      DirtyFlag = true;
    }

    [[nodiscard]] auto
    GetParent() const -> entt::entity
    {
      return Parent;
    }

    /// @brief Returns the model matrix of the entity. A composite matrix of the
    /// translation, scale and rotation matrices.
    /// @return The model matrix as a 4x4 matrix.
    [[nodiscard]] auto
    GetMatrix() -> glm::mat4x4
    {
      if (DirtyFlag)
      {
        UpdateMatrix();
      }

      return CachedMatrix;
    }

    void
    SetMatrix(glm::mat4 matrix)
    {
      glm::vec3 skew;
      glm::vec4 perspective;
      glm::quat rotQuat;
      glm::vec3 trans, scale;

      glm::decompose(matrix, scale, rotQuat, trans, skew, perspective);

      glm::vec3 rotation = WrapEuler(glm::degrees(glm::eulerAngles(rotQuat)));

      Position = trans;
      Rotation = rotation;
      Scale = scale;
      DirtyFlag = true;
    }

    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json serializedTransformComponent;
      serializedTransformComponent["Position"] = Position;

      serializedTransformComponent["Rotation"] = Rotation;

      serializedTransformComponent["Scale"] = Scale;
      return serializedTransformComponent;
    }
  };
}