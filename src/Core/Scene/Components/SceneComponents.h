#pragma once
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.h"
#include "Utilities/ISerializable.h"
#include "pch.h"

#include <entt/entt.hpp>
#include <memory>
#include <nlohmann/json_fwd.hpp>
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
      Position = { json["Position"]["x"],
                   json["Position"]["y"],
                   json["Position"]["z"] };
      Rotation = { json["Rotation"]["x"],
                   json["Rotation"]["y"],
                   json["Rotation"]["z"] };
      Scale = { json["Scale"]["x"], json["Scale"]["y"], json["Scale"]["z"] };
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
      return glm::toMat4(glm::quat(DEG_2_RAD * Rotation));
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
    Serialize() override
    {
      nlohmann::json serializedTransformComponent;
      serializedTransformComponent["Position"]["x"] = Position.x;
      serializedTransformComponent["Position"]["y"] = Position.y;
      serializedTransformComponent["Position"]["z"] = Position.z;

      serializedTransformComponent["Rotation"]["x"] = Rotation.x;
      serializedTransformComponent["Rotation"]["y"] = Rotation.y;
      serializedTransformComponent["Rotation"]["z"] = Rotation.z;

      serializedTransformComponent["Scale"]["x"] = Scale.x;
      serializedTransformComponent["Scale"]["y"] = Scale.y;
      serializedTransformComponent["Scale"]["z"] = Scale.z;
      // j["parent"] = parent;
      // j["children"] = children;
      return serializedTransformComponent;
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
      : Type(static_cast<LIGHT_TYPE>(json["Type"]))
      , Color({ json["Color"][0], json["Color"][1], json["Color"][2] })
      , Attenuation(json["Attenuation"])
      , Radius(json["Radius"])
      , OpeningAngle(json["OpeningAngle"])
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
    Serialize() override
    {
      nlohmann::json serializedLightComponent;
      serializedLightComponent["Type"] = (int)Type;

      serializedLightComponent["LightColor"]["r"] = (int)Color.r;
      serializedLightComponent["LightColor"]["g"] = (int)Color.g;
      serializedLightComponent["LightColor"]["b"] = (int)Color.b;

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
    bool canCastShadow;

  public:
    MeshRendererComponent() = default;
    MeshRendererComponent(
      std::unique_ptr<IAssetReference>                modelAsset,
      std::map<int, std::unique_ptr<IAssetReference>> materials)
      : modelAsset(std::move(modelAsset))
      , materialAssets(std::move(materials))
    {
    }

    std::unique_ptr<IAssetReference>&
    GetModelAsset()
    {
      return modelAsset;
    }

    const std::unique_ptr<IAssetReference>&
    GetModelAsset() const
    {
      return modelAsset;
    }

    std::map<int, std::unique_ptr<IAssetReference>>&
    MaterialAssets()
    {
      return materialAssets;
    }

    const std::map<int, std::unique_ptr<IAssetReference>>&
    GetMaterialAssets() const
    {
      return materialAssets;
    }

    bool&
    IsHidden()
    {
      return isHidden;
    }

    std::unique_ptr<IMeshBuffer>&
    IdMesh()
    {
      return idMeshBuffer;
    }

    nlohmann::json
    Serialize() override
    {
      nlohmann::json serializedMeshRendererComponent;
      if (modelAsset)
      {
        serializedMeshRendererComponent["Model"] =
          modelAsset->GetUID().ToString();
      }
      else
      {
        serializedMeshRendererComponent["Model"] = "";
      }

      int materialCount = 0;

      for (const auto& [index, material] : materialAssets)
      {
        serializedMeshRendererComponent["Materials"][std::to_string(index)] =
          material ? material->GetUID().ToString() : "null";
      }

      serializedMeshRendererComponent["Hidden"] = isHidden;

      return serializedMeshRendererComponent;
    }
  };
}