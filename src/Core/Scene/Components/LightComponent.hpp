#pragma once

#include "Core/Asset/AssetReference/IAssetReference.hpp"
#include "Core/Rendering/LightTypes.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.hpp"
#include "Core/Scene/Camera/ICamera.hpp"
#include "Core/UUID.hpp"
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
  /// @brief Entity component holding light properties.
  struct LightComponent : public ISerializable
  {
    /// @brief The type of light this component represents.
    LightType Type = LightType::Directional;

    /// @brief The color of the light as a 3D vector (R,G,B).
    glm::vec3 Color = glm::vec3(1.0F);

    /// @brief The attenuation (or intensity) of the light.
    float Attenuation = 4.0F;

    /// @brief The radius of a point light.
    float Radius = 15.0F;

    /// @brief The angle at which the spot light shines.
    float OpeningAngle = 33.0F;

    LightComponent() = default;

    LightComponent(const nlohmann::json& json)
      : Type(json["Type"].get<LightType>())
      , Color(json["LightColor"].get<glm::vec3>())
      , Attenuation(json["Attenuation"].get<float>())
      , Radius(json["Radius"].get<float>())
      , OpeningAngle(json["OpeningAngle"].get<float>())
    {
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
}