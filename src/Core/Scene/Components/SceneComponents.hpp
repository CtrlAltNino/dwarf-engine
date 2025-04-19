#pragma once

#include "Core/Asset/AssetReference/IAssetReference.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.hpp"
#include "Core/UUID.hpp"
#include "Utilities/ISerializable.hpp"
#include "Utilities/JsonHelper/JsonHelper.hpp"
#include <entt/entt.hpp>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <string>

namespace Dwarf
{
  /// @brief A component holding a transform.
  struct TransformComponent : public ISerializable
  {
  private:
#define RAD_2_DEG (180.0f / std::numbers::pi_v<float>)
#define DEG_2_RAD (std::numbers::pi_v<float> / 180.0f)

    /// @brief Position of the entity.
    glm::vec3 Position = { 0.0F, 0.0F, 0.0F };

    /// @brief Rotation of the entity as euler angles.
    glm::vec3 Rotation = { 0.0F, 0.0F, 0.0F };

    /// @brief Scale of the entity.
    glm::vec3 Scale = { 1.0F, 1.0F, 1.0F };

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
      Position = json["Position"].get<glm::vec3>();
      Rotation = json["Rotation"].get<glm::vec3>();
      Scale = json["Scale"].get<glm::vec3>();
      // parent = json["parent"];
      // children = json["children"];
    }

    // ========== Getters ==========

    /// @brief Returns the position of the entity.
    /// @return The position as a 3D vector.
    auto
    GetPosition() -> glm::vec3&
    {
      return Position;
    }

    [[nodiscard]] auto
    GetPosition() const -> const glm::vec3&
    {
      return Position;
    }

    /// @brief Returns the rotation of the entity as euler angles.
    /// @return The euler angles as a 3D vector.
    auto
    GetEulerAngles() -> glm::vec3&
    {
      return Rotation;
    }

    [[nodiscard]] auto
    GetEulerAngles() const -> const glm::vec3&
    {
      return Rotation;
    }

    /// @brief Returns the rotation of the entity as a matrix.
    /// @return The rotations as a 4x4 matrix.
    [[nodiscard]] auto
    GetRotationMatrix() const -> glm::mat4
    {
      return glm::toMat4(glm::quat(DEG_2_RAD * Rotation));
    }

    /// @brief Returns the scale of the entity.
    /// @return The scale as a 3D vector.
    auto
    GetScale() -> glm::vec3&
    {
      return Scale;
    }

    [[nodiscard]] auto
    GetScale() const -> const glm::vec3&
    {
      return Scale;
    }

    /// @brief Returns the entity handle of the parent entity.
    /// @return The parent entity handle.
    auto
    GetParent() -> entt::entity&
    {
      return Parent;
    }

    [[nodiscard]] auto
    GetParent() const -> const entt::entity&
    {
      return Parent;
    }

    auto
    GetChildren() -> std::vector<entt::entity>&
    {
      return Children;
    }

    [[nodiscard]] auto
    GetChildren() const -> const std::vector<entt::entity>&
    {
      return Children;
    }

    /// @brief Returns the vector that points into the entity's forward
    /// direction.
    /// @return The forward vector as a 3D vector.
    [[nodiscard]] auto
    GetForward() const -> glm::vec3
    {
      glm::mat4 rotationMatrix = GetRotationMatrix();
      glm::vec3 forwardVector(
        rotationMatrix[2][0], rotationMatrix[2][1], rotationMatrix[2][2]);
      return glm::normalize(forwardVector);
    }

    /// @brief Returns the vector that points into the entity's upwards
    /// direction.
    /// @return The up vector as a 3D vector.
    [[nodiscard]] auto
    GetUp() const -> glm::vec3
    {
      return GetRotationMatrix() * glm::vec4(0, 1, 0, 1);
    }

    /// @brief Returns the vector that points into the entity's right side
    /// direction.
    /// @return The right vector as a 3D vector.
    [[nodiscard]] auto
    GetRight() const -> glm::vec3
    {
      return GetRotationMatrix() * glm::vec4(1, 0, 0, 1);
    }

    /// @brief Returns the model matrix of the entity. A composite matrix of the
    /// translation, scale and rotation matrices.
    /// @return The model matrix as a 4x4 matrix.
    [[nodiscard]] auto
    GetModelMatrix() const -> glm::mat4x4
    {
      glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0F), Position);
      glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0F), Scale);
      glm::mat4 rotationMatrix = GetRotationMatrix();

      return translationMatrix * rotationMatrix * scaleMatrix;
    }

    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json serializedTransformComponent;
      serializedTransformComponent["Position"] = Position;

      serializedTransformComponent["Rotation"] = Rotation;

      serializedTransformComponent["Scale"] = Scale;
      // j["parent"] = parent;
      // j["children"] = children;
      return serializedTransformComponent;
    }
  };

  /// @brief Entity component holding light properties.
  struct LightComponent : public ISerializable
  {
    /// @brief Enum representing light types.
    enum class LIGHT_TYPE : uint8_t
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
    glm::vec3 Color = glm::vec3(1.0F);

    /// @brief The attenuation (or intensity) of the light.
    float Attenuation = 4.0F;

    /// @brief The radius of a point light.
    float Radius = 15.0F;

    /// @brief The angle at which the spot light shines.
    float OpeningAngle = 33.0F;

  public:
    LightComponent() = default;

    LightComponent(const nlohmann::json& json)
      : Type(static_cast<LIGHT_TYPE>(json["Type"].get<LIGHT_TYPE>()))
      , Color(json["LightColor"].get<glm::vec3>())
      , Attenuation(json["Attenuation"].get<float>())
      , Radius(json["Radius"].get<float>())
      , OpeningAngle(json["OpeningAngle"].get<float>())
    {
    }

    /// @brief Returns the type of light this component represents.
    /// @return The light type.
    auto
    GetType() -> LIGHT_TYPE&
    {
      return Type;
    }

    /// @brief Returns the color of the light.
    /// @return The color as a 3D vector.
    auto
    GetColor() -> glm::vec3&
    {
      return Color;
    }

    /// @brief Returns the attenuation of the light.
    /// @return The attenuation as a float.
    auto
    GetAttenuation() -> float&
    {
      return Attenuation;
    }

    /// @brief Returns the radius of the point light.
    /// @return The radius as a float.
    auto
    GetRadius() -> float&
    {
      return Radius;
    }

    /// @brief Returns the opening angle of the spot light.
    /// @return The opening angle as a float.
    auto
    GetOpeningAngle() -> float&
    {
      return OpeningAngle;
    }

    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json serializedLightComponent;
      serializedLightComponent["Type"] = (int)Type;

      serializedLightComponent["LightColor"] = Color;

      serializedLightComponent["Attenuation"] = (int)Attenuation;

      serializedLightComponent["Radius"] = (int)Radius;

      serializedLightComponent["OpeningAngle"] = (int)OpeningAngle;

      return serializedLightComponent;
    }
  };

  /// @brief A component holding meshes and their corresponding materials, as
  /// well as other information on how to render these meshes.
  struct MeshRendererComponent : public ISerializable
  {
  private:
    bool isHidden = false;
    /// @brief ID of the mesh asset.
    std::unique_ptr<IAssetReference> modelAsset = nullptr;
    std::unique_ptr<IMeshBuffer>     idMeshBuffer = nullptr;

    /// @brief The materials with which the model is to be rendered. The list
    /// index of the materials corresponds to the material index of the
    /// submeshes.
    std::map<int, std::unique_ptr<IAssetReference>> materialAssets;

    /// @brief Flag that decides if this model should be used in the shadow cast
    /// pass.
    bool canCastShadow = true;

  public:
    MeshRendererComponent() = default;
    MeshRendererComponent(
      std::unique_ptr<IAssetReference>                modelAsset,
      std::map<int, std::unique_ptr<IAssetReference>> materials)
      : modelAsset(std::move(modelAsset))
      , materialAssets(std::move(materials))
    {
    }

    auto
    GetModelAsset() -> std::unique_ptr<IAssetReference>&
    {
      return modelAsset;
    }

    [[nodiscard]] auto
    GetModelAsset() const -> const std::unique_ptr<IAssetReference>&
    {
      return modelAsset;
    }

    auto
    MaterialAssets() -> std::map<int, std::unique_ptr<IAssetReference>>&
    {
      return materialAssets;
    }

    [[nodiscard]] auto
    GetMaterialAssets() const
      -> const std::map<int, std::unique_ptr<IAssetReference>>&
    {
      return materialAssets;
    }

    auto
    IsHidden() -> bool&
    {
      return isHidden;
    }

    auto
    IdMesh() -> std::unique_ptr<IMeshBuffer>&
    {
      return idMeshBuffer;
    }

    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json serializedMeshRendererComponent;
      if (modelAsset)
      {
        serializedMeshRendererComponent["Model"] =
          modelAsset->GetUID().toString();
      }
      else
      {
        serializedMeshRendererComponent["Model"] = "";
      }

      int materialCount = 0;

      for (const auto& [index, material] : materialAssets)
      {
        serializedMeshRendererComponent["Materials"][std::to_string(index)] =
          material ? material->GetUID().toString() : "null";
      }

      serializedMeshRendererComponent["Hidden"] = isHidden;

      return serializedMeshRendererComponent;
    }
  };
}