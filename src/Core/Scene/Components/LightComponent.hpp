#pragma once

#include "Core/Rendering/LightTypes.hpp"
#include "Utilities/ISerializable.hpp"
#include "Utilities/JsonHelper/JsonHelper.hpp"
#include <entt/entt.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <nlohmann/json_fwd.hpp>

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

    bool CastsShadows = true;

    bool Enabled = true;

    LightComponent() = default;

    LightComponent(const nlohmann::json& json)
    {
      if (json.contains("Type"))
      {
        Type = json["Type"].get<LightType>();
      }

      if (json.contains("LightColor"))
      {
        Color = json["LightColor"].get<glm::vec3>();
      }

      if (json.contains("Attenuation"))
      {
        Attenuation = json["Attenuation"].get<float>();
      }

      if (json.contains("Radius"))
      {
        Radius = json["Radius"].get<float>();
      }

      if (json.contains("OpeningAngle"))
      {
        OpeningAngle = json["OpeningAngle"].get<float>();
      }

      if (json.contains("CastsShadows"))
      {
        CastsShadows = json["CastsShadows"].get<bool>();
      }

      if (json.contains("Enabled"))
      {
        Enabled = json["Enabled"].get<bool>();
      }
    }

    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json serializedLightComponent;
      serializedLightComponent["Type"] = (int)Type;

      serializedLightComponent["LightColor"] = Color;

      serializedLightComponent["Attenuation"] = Attenuation;

      serializedLightComponent["Radius"] = Radius;

      serializedLightComponent["OpeningAngle"] = OpeningAngle;

      serializedLightComponent["CastsShadows"] = CastsShadows;

      serializedLightComponent["Enabled"] = Enabled;

      return serializedLightComponent;
    }
  };
}