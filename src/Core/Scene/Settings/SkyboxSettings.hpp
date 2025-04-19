#pragma once

#include "Core/Rendering/SkyboxTypes.hpp"
#include "Core/Rendering/Texture/ITexture.h"
#include "Core/UUID.h"
#include "Utilities/ISerializable.h"
#include "Utilities/JsonHelper/JsonHelper.h"

namespace Dwarf
{
  struct SkyboxSettings : public ISerializable
  {
    SkyboxSource        Type = SkyboxSource::Color;
    glm::vec3           Color = glm::vec3(0.0F, 0.0F, 0.0F);
    std::optional<UUID> SkyboxMaterial = std::nullopt;
    std::optional<UUID> CubeMap = std::nullopt;

    /// @brief Constructor.
    SkyboxSettings() = default;

    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    SkyboxSettings(nlohmann::json json)
    {
      if (json.contains("Type"))
      {
        Type = json["Type"].get<SkyboxSource>();
      }
      if (json.contains("Color"))
      {
        Color = json["Color"].get<glm::vec3>();
      }
      if (json.contains("SkyboxMaterial"))
      {
        SkyboxMaterial = UUID(json.at("SkyboxMaterial").get<std::string>());
      }
      if (json.contains("CubeMap"))
      {
        CubeMap = UUID(json.at("CubeMap").get<std::string>());
      }
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["Type"] = Type;
      json["Color"] = Color;
      json["SkyboxMaterial"] =
        SkyboxMaterial.has_value() ? SkyboxMaterial->Serialize() : "";
      json["CubeMap"] = CubeMap.has_value() ? CubeMap->Serialize() : "";
      return json;
    }
  };
}