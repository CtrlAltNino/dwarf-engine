#pragma once
#include "Utilities/ISerializable.h"
#include "pch.h"

#include <entt/entt.hpp>
#include "Core/UUID.h"

namespace Dwarf
{
  /// @brief A component holding a transform.
  struct TransformComponent : public ISerializable
  {
#define RAD_2_DEG (180.0f / std::numbers::pi_v<float>)
#define DEG_2_RAD (std::numbers::pi_v<float> / 180.0f)

    /// @brief Position of the entity.
    glm::vec3 position = { 0.0f, 0.0f, 0.0f };

    /// @brief Rotation of the entity as euler angles.
    glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };

    /// @brief Scale of the entity.
    glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

    /// @brief Entity handle of the hierarchical parent entity.
    entt::entity parent = entt::null;

    /// @brief List of entity handles that are child entities.
    std::vector<entt::entity> children;

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    explicit TransformComponent(glm::vec3 pos)
      : position(pos)
    {
    }
    TransformComponent(glm::vec3 pos, glm::vec3 rot)
      : position(pos)
      , rotation(rot)
    {
    }

    TransformComponent(nlohmann::json json)
    {
      position = { json["position"][0],
                   json["position"][1],
                   json["position"][2] };
      rotation = { json["rotation"][0],
                   json["rotation"][1],
                   json["rotation"][2] };
      scale = { json["scale"][0], json["scale"][1], json["scale"][2] };
      // parent = json["parent"];
      // children = json["children"];
    }

    // ========== Getters ==========

    /// @brief Returns the position of the entity.
    /// @return The position as a 3D vector.
    glm::vec3
    getPosition() const
    {
      return position;
    }

    /// @brief Returns the rotation of the entity as euler angles.
    /// @return The euler angles as a 3D vector.
    glm::vec3
    getEulerAngles() const
    {
      return rotation;
    }

    /// @brief Returns the rotation of the entity as a matrix.
    /// @return The rotations as a 4x4 matrix.
    glm::mat4
    getRotationMatrix() const
    {
      glm::mat4 rotationMatrix(1.0f); // Identity matrix

      // Apply pitch, yaw, and roll in the specified order
      rotationMatrix = glm::rotate(
        rotationMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
      rotationMatrix = glm::rotate(
        rotationMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
      rotationMatrix = glm::rotate(
        rotationMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

      return glm::toMat4(glm::quat(rotation));
    }

    /// @brief Returns the scale of the entity.
    /// @return The scale as a 3D vector.
    glm::vec3
    getScale() const
    {
      return scale;
    }

    /// @brief Returns the vector that points into the entity's forward
    /// direction.
    /// @return The forward vector as a 3D vector.
    glm::vec3
    getForward() const
    {
      glm::mat4 rotationMatrix = getRotationMatrix();
      glm::vec3 forwardVector(
        rotationMatrix[2][0], rotationMatrix[2][1], rotationMatrix[2][2]);
      return glm::normalize(forwardVector);
    }

    /// @brief Returns the vector that points into the entity's upwards
    /// direction.
    /// @return The up vector as a 3D vector.
    glm::vec3
    getUp() const
    {
      return getRotationMatrix() * glm::vec4(0, 1, 0, 1);
    }

    /// @brief Returns the vector that points into the entity's right side
    /// direction.
    /// @return The right vector as a 3D vector.
    glm::vec3
    getRight() const
    {
      return getRotationMatrix() * glm::vec4(1, 0, 0, 1);
    }

    /// @brief Returns the model matrix of the entity. A composite matrix of the
    /// translation, scale and rotation matrices.
    /// @return The model matrix as a 4x4 matrix.
    glm::mat4x4
    getModelMatrix() const
    {
      glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
      glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
      glm::mat4 rotationMatrix = getRotationMatrix();

      return translationMatrix * rotationMatrix * scaleMatrix;
    }

    nlohmann::json
    Serialize() const override
    {
      nlohmann::json j;
      j["position"] = { position.x, position.y, position.z };
      j["rotation"] = { rotation.x, rotation.y, rotation.z };
      j["scale"] = { scale.x, scale.y, scale.z };
      // j["parent"] = parent;
      // j["children"] = children;
      return j;
    }
  };

  /// @brief Entity component holding light properties.
  struct LightComponent
  {
    /// @brief Enum representing light types.
    enum class LIGHT_TYPE
    {
      DIRECTIONAL,
      POINT_LIGHT,
      SPOT_LIGHT
    };

    /// @brief Names of the light types.
    static inline std::array<const char*, 3> LIGHT_TYPE_NAMES = { "Directional",
                                                                  "Point",
                                                                  "Spot" };

    /// @brief The type of light this component represents.
    LIGHT_TYPE type = LIGHT_TYPE::DIRECTIONAL;

    /// @brief The color of the light as a 3D vector (R,G,B).
    glm::vec3 lightColor = glm::vec3(1.0f);

    /// @brief The attenuation (or intensity) of the light.
    float attenuation = 4.0f;

    /// @brief The radius of a point light.
    float radius = 15.0f;

    /// @brief The angle at which the spot light shines.
    float openingAngle = 33;

    LightComponent() = default;
  };

  /// @brief A component holding meshes and their corresponding materials, as
  /// well as other information on how to render these meshes.
  struct MeshRendererComponent
  {
  private:
    /// @brief ID of the mesh asset.
    UUID meshAsset;

    /// @brief The materials with which the model is to be rendered. The list
    /// index of the materials corresponds to the material index of the
    /// submeshes.
    std::vector<UUID> materialAssets;

  public:
    MeshRendererComponent() = default;
    MeshRendererComponent(const MeshRendererComponent&) = default;
    MeshRendererComponent(const UUID& mesh, const std::vector<UUID>& materials)
      : meshAsset(mesh)
      , materialAssets(materials)
    {
    }
    /// @brief Flag that decides if this model should be used in the shadow cast
    /// pass.
    bool canCastShadow;

    UUID&
    GetMeshAsset()
    {
      return meshAsset;
    }

    std::vector<UUID>&
    GetMaterialAssets()
    {
      return materialAssets;
    }
  };
}