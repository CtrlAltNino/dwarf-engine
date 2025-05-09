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
    SkyboxSource          Type = SkyboxSource::Color;
    glm::vec3             Color = glm::vec3(0.0F, 0.0F, 0.0F);
    std::optional<UUID>   SkyboxMaterial = std::nullopt;
    std::optional<UUID>   Cubemap = std::nullopt;
    CubemapResolutionEnum CubemapResolution = CubemapResolutionEnum::Medium;
    float                 Exposure = 1;
    float                 CubemapRotation = 0;

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
      if (json.contains("Cubemap") &&
          !json.at("Cubemap").get<std::string>().empty())
      {
        Cubemap = UUID(json.at("Cubemap").get<std::string>());
      }
      if (json.contains("CubemapResolution"))
      {
        CubemapResolution =
          json.at("CubemapResolution").get<CubemapResolutionEnum>();
      }
      if (json.contains("Exposure"))
      {
        Exposure = json.at("Exposure").get<float>();
      }
      if (json.contains("CubemapRotation"))
      {
        CubemapRotation = json.at("CubemapRotation").get<float>();
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
      json["Cubemap"] = Cubemap.has_value() ? Cubemap->Serialize() : "";
      json["CubemapResolution"] = CubemapResolution;
      json["Exposure"] = Exposure;
      json["CubemapRotation"] = CubemapRotation;
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

    [[nodiscard]] auto
    GetType() const -> SkyboxSource
    {
      return Type;
    }

    void
    SetType(SkyboxSource type)
    {
      Type = type;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetColor() const -> glm::vec3
    {
      return Color;
    }

    void
    SetColor(glm::vec3 color)
    {
      Color = color;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetSkyboxMaterial() const -> std::optional<UUID>
    {
      return SkyboxMaterial;
    }

    void
    SetSkyboxMaterial(std::optional<UUID> materialId)
    {
      SkyboxMaterial = std::move(materialId);
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetHdri() const -> std::optional<UUID>
    {
      return Cubemap;
    }

    auto
    SetHdri(std::optional<UUID> hdri)
    {
      Cubemap = std::move(hdri);
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetCubemapResolution() const -> CubemapResolutionEnum
    {
      return CubemapResolution;
    }

    void
    SetCubemapResolution(CubemapResolutionEnum cubemapResolution)
    {
      CubemapResolution = cubemapResolution;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetExposure() const -> float
    {
      return Exposure;
    }

    void
    SetExposure(float exposure)
    {
      Exposure = exposure;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetCubemapRotation() const -> float
    {
      return CubemapRotation;
    }

    void
    SetCubemapRotation(float rotation)
    {
      CubemapRotation = rotation;
      NotifyObservers();
    }
  };
}