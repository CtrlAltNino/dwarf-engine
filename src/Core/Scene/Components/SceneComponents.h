#pragma once
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Utilities/ISerializable.h"
#include "pch.h"

#include <entt/entt.hpp>
#include <memory>
#include <string>
#include "Core/UUID.h"

namespace Dwarf
{
  /// @brief A component holding a transform.
  struct TransformComponent : public ISerializable
  {
  private:
#define RAD_2_DEG (180.0f / std::numbers::pi_v<float>)
#define DEG_2_RAD (std::numbers::pi_v<float> / 180.0f)

    /// @brief Position of the entity.
    glm::vec3 Position = { 0.0f, 0.0f, 0.0f };

    /// @brief Rotation of the entity as euler angles.
    glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };

    /// @brief Scale of the entity.
    glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

    /// @brief Entity handle of the hierarchical parent entity.
    entt::entity Parent = entt::null;

    /// @brief List of entity handles that are child entities.
    std::vector<entt::entity> Children;

  public:
    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    explicit TransformComponent(glm::vec3 pos)
      : Position(pos)
    {
    }
    TransformComponent(glm::vec3 pos, glm::vec3 rot)
      : Position(pos)
      , Rotation(rot)
    {
    }

    TransformComponent(const nlohmann::json& json)
    {
      Position = { json["position"][0],
                   json["position"][1],
                   json["position"][2] };
      Rotation = { json["rotation"][0],
                   json["rotation"][1],
                   json["rotation"][2] };
      Scale = { json["scale"][0], json["scale"][1], json["scale"][2] };
      // parent = json["parent"];
      // children = json["children"];
    }

    // ========== Getters ==========

    /// @brief Returns the position of the entity.
    /// @return The position as a 3D vector.
    glm::vec3&
    GetPosition()
    {
      return Position;
    }

    const glm::vec3&
    GetPosition() const
    {
      return Position;
    }

    /// @brief Returns the rotation of the entity as euler angles.
    /// @return The euler angles as a 3D vector.
    glm::vec3&
    GetEulerAngles()
    {
      return Rotation;
    }

    const glm::vec3&
    GetEulerAngles() const
    {
      return Rotation;
    }

    /// @brief Returns the rotation of the entity as a matrix.
    /// @return The rotations as a 4x4 matrix.
    glm::mat4
    GetRotationMatrix() const
    {
      glm::mat4 rotationMatrix(1.0f); // Identity matrix

      // Apply pitch, yaw, and roll in the specified order
      rotationMatrix = glm::rotate(
        rotationMatrix, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
      rotationMatrix = glm::rotate(
        rotationMatrix, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
      rotationMatrix = glm::rotate(
        rotationMatrix, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

      return glm::toMat4(glm::quat(Rotation));
    }

    /// @brief Returns the scale of the entity.
    /// @return The scale as a 3D vector.
    glm::vec3&
    GetScale()
    {
      return Scale;
    }

    const glm::vec3&
    GetScale() const
    {
      return Scale;
    }

    /// @brief Returns the entity handle of the parent entity.
    /// @return The parent entity handle.
    entt::entity&
    GetParent()
    {
      return Parent;
    }

    const entt::entity&
    GetParent() const
    {
      return Parent;
    }

    std::vector<entt::entity>&
    GetChildren()
    {
      return Children;
    }

    const std::vector<entt::entity>&
    GetChildren() const
    {
      return Children;
    }

    /// @brief Returns the vector that points into the entity's forward
    /// direction.
    /// @return The forward vector as a 3D vector.
    glm::vec3
    GetForward() const
    {
      glm::mat4 rotationMatrix = GetRotationMatrix();
      glm::vec3 forwardVector(
        rotationMatrix[2][0], rotationMatrix[2][1], rotationMatrix[2][2]);
      return glm::normalize(forwardVector);
    }

    /// @brief Returns the vector that points into the entity's upwards
    /// direction.
    /// @return The up vector as a 3D vector.
    glm::vec3
    GetUp() const
    {
      return GetRotationMatrix() * glm::vec4(0, 1, 0, 1);
    }

    /// @brief Returns the vector that points into the entity's right side
    /// direction.
    /// @return The right vector as a 3D vector.
    glm::vec3
    GetRight() const
    {
      return GetRotationMatrix() * glm::vec4(1, 0, 0, 1);
    }

    /// @brief Returns the model matrix of the entity. A composite matrix of the
    /// translation, scale and rotation matrices.
    /// @return The model matrix as a 4x4 matrix.
    glm::mat4x4
    GetModelMatrix() const
    {
      glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Position);
      glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), Scale);
      glm::mat4 rotationMatrix = GetRotationMatrix();

      return translationMatrix * rotationMatrix * scaleMatrix;
    }

    nlohmann::json
    Serialize() const override
    {
      nlohmann::json j;
      j["position"] = { Position.x, Position.y, Position.z };
      j["rotation"] = { Rotation.x, Rotation.y, Rotation.z };
      j["scale"] = { Scale.x, Scale.y, Scale.z };
      // j["parent"] = parent;
      // j["children"] = children;
      return j;
    }
  };

  /// @brief Entity component holding light properties.
  struct LightComponent : public ISerializable
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

  private:
    /// @brief The type of light this component represents.
    LIGHT_TYPE Type = LIGHT_TYPE::DIRECTIONAL;

    /// @brief The color of the light as a 3D vector (R,G,B).
    glm::vec3 Color = glm::vec3(1.0f);

    /// @brief The attenuation (or intensity) of the light.
    float Attenuation = 4.0f;

    /// @brief The radius of a point light.
    float Radius = 15.0f;

    /// @brief The angle at which the spot light shines.
    float OpeningAngle = 33;

  public:
    LightComponent() = default;

    LightComponent(const nlohmann::json& json)
      : Type(static_cast<LIGHT_TYPE>(json["type"]))
      , Color({ json["color"][0], json["color"][1], json["color"][2] })
      , Attenuation(json["attenuation"])
      , Radius(json["radius"])
      , OpeningAngle(json["openingAngle"])
    {
    }

    /// @brief Returns the type of light this component represents.
    /// @return The light type.
    LIGHT_TYPE&
    GetType()
    {
      return Type;
    }

    /// @brief Returns the color of the light.
    /// @return The color as a 3D vector.
    glm::vec3&
    GetColor()
    {
      return Color;
    }

    /// @brief Returns the attenuation of the light.
    /// @return The attenuation as a float.
    float&
    GetAttenuation()
    {
      return Attenuation;
    }

    /// @brief Returns the radius of the point light.
    /// @return The radius as a float.
    float&
    GetRadius()
    {
      return Radius;
    }

    /// @brief Returns the opening angle of the spot light.
    /// @return The opening angle as a float.
    float&
    GetOpeningAngle()
    {
      return OpeningAngle;
    }

    nlohmann::json
    Serialize() const override
    {
      nlohmann::json j;
      j["type"] = static_cast<int>(Type);
      j["color"] = { Color.x, Color.y, Color.z };
      j["attenuation"] = Attenuation;
      j["radius"] = Radius;
      j["openingAngle"] = OpeningAngle;
      return j;
    }
  };

  /// @brief A component holding meshes and their corresponding materials, as
  /// well as other information on how to render these meshes.
  struct MeshRendererComponent : public ISerializable
  {
  private:
    /// @brief ID of the mesh asset.
    std::optional<IAssetReference<ModelAsset>> modelAsset = std::nullopt;

    /// @brief The materials with which the model is to be rendered. The list
    /// index of the materials corresponds to the material index of the
    /// submeshes.
    std::vector<std::optional<IAssetReference<MaterialAsset>>>
      materialAssets = {};

  public:
    MeshRendererComponent() = default;
    MeshRendererComponent(
      std::optional<IAssetReference<ModelAsset>>&&                modelAsset,
      std::vector<std::optional<IAssetReference<MaterialAsset>>>& materials)
      : modelAsset(std::move(modelAsset))
    {
      materialAssets.clear();
      materialAssets.reserve(materials.size());
      for (auto& material : materialAssets)
      {
        materialAssets.push_back(std::move(material));
      }
    }

    /// @brief Flag that decides if this model should be used in the shadow cast
    /// pass.
    bool canCastShadow;

    std::optional<IAssetReference<ModelAsset>>&
    GetModelAsset()
    {
      return modelAsset;
    }

    const std::optional<IAssetReference<ModelAsset>>&
    GetModelAsset() const
    {
      return modelAsset;
    }

    std::vector<std::optional<IAssetReference<MaterialAsset>>>&
    MaterialAssets()
    {
      return materialAssets;
    }

    const std::vector<std::optional<IAssetReference<MaterialAsset>>>&
    GetMaterialAssets() const
    {
      return materialAssets;
    }

    nlohmann::json
    Serialize() const override
    {
      nlohmann::json j;
      j["modelAsset"] =
        modelAsset.has_value() ? modelAsset.value().GetUID().ToString() : "";
      j["materialAssets"] = nlohmann::json::array();

      for (auto& material : materialAssets)
      {
        if (material.has_value())
        {
          j["materialAssets"].push_back(material.value().GetUID().ToString());
        }
        else
        {
          j["materialAssets"].push_back("");
        }
      }
      return j;
    }
  };
}