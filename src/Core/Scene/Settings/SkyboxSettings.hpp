#pragma once

#include "Core/Rendering/SkyboxTypes.hpp"
#include "Core/Rendering/Texture/ITexture.hpp"
#include "Core/UUID.hpp"
#include "ISceneSettingsObserver.hpp"
#include "Utilities/ISerializable.hpp"
#include "Utilities/JsonHelper/JsonHelper.hpp"
#include <glm/ext/vector_float3.hpp>
#include <optional>
#include <utility>

namespace Dwarf
{
  struct SkyboxSettings : public ISerializable
  {
  private:
    std::reference_wrapper<std::vector<ISceneSettingsObserver*>> Observers;
    SkyboxSource        Type = SkyboxSource::Color;
    glm::vec3           Color = glm::vec3(0.0F, 0.0F, 0.0F);
    std::optional<UUID> SkyboxMaterial = std::nullopt;
    std::optional<UUID> CubeMap = std::nullopt;

  public:
    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    SkyboxSettings(std::vector<ISceneSettingsObserver*>& observers,
                   nlohmann::json                        json = "")
      : Observers(observers)
    {
      if (json.contains("Type"))
      {
        Type = json["Type"].get<SkyboxSource>();
      }
      if (json.contains("Color"))
      {
        Color = json["Color"].get<glm::vec3>();
      }
      if (json.contains("SkyboxMaterial") &&
          !json.at("SkyboxMaterial").get<std::string>().empty())
      {
        SkyboxMaterial = UUID(json.at("SkyboxMaterial").get<std::string>());
      }
      if (json.contains("CubeMap") &&
          !json.at("CubeMap").get<std::string>().empty())
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

    void
    NotifyObservers()
    {
      for (auto* observer : Observers.get())
      {
        observer->OnSkyboxSettingsChanged();
      }
    }

    auto
    GetType() -> SkyboxSource
    {
      return Type;
    }

    void
    SetType(SkyboxSource type)
    {
      Type = type;
      NotifyObservers();
    }

    auto
    GetColor() -> glm::vec3
    {
      return Color;
    }

    void
    SetColor(glm::vec3 color)
    {
      Color = color;
      NotifyObservers();
    }

    auto
    GetSkyboxMaterial() -> std::optional<UUID>
    {
      return SkyboxMaterial;
    }

    void
    SetSkyboxMaterial(std::optional<UUID> materialId)
    {
      SkyboxMaterial = std::move(materialId);
      NotifyObservers();
    }

    auto
    GetHdri() -> std::optional<UUID>
    {
      return CubeMap;
    }

    auto
    SetHdri(std::optional<UUID> hdri)
    {
      CubeMap = std::move(hdri);
      NotifyObservers();
    }
  };
}