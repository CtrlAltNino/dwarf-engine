#pragma once

#include "Core/Rendering/FogTypes.hpp"
#include "Core/Scene/Settings/ISceneSettingsObserver.hpp"
#include "Utilities/ISerializable.hpp"
#include "Utilities/JsonHelper/JsonHelper.hpp"

namespace Dwarf
{
  /// @brief Struct that represents the fog settings of a scene.
  struct FogSettings : public ISerializable
  {
  private:
    std::reference_wrapper<std::vector<ISceneSettingsObserver*>> Observers;
    /// @brief Type of the fog.
    FogType Type = FogType::Linear;
    /// @brief Color of the fog.
    glm::vec3 Color = { 0.3F, 0.3F, 0.3F };
    /// @brief Starting distance of the fog.
    float Start = 20.0F;
    /// @brief Ending distance of the fog (only for linear fog).
    float End = 50.0F;
    float Density = 3.0F;

  public:
    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    FogSettings(std::vector<ISceneSettingsObserver*>& observers,
                nlohmann::json                        serializedSettings = "")
      : Observers(observers)
    {
      if (serializedSettings.contains("Color"))
      {
        Color = serializedSettings["Color"].get<glm::vec3>();
      }
      if (serializedSettings.contains("Start"))
      {
        Start = serializedSettings["Start"].get<float>();
      }
      if (serializedSettings.contains("End"))
      {
        End = serializedSettings["End"].get<float>();
      }
      if (serializedSettings.contains("title"))
      {
        Type = serializedSettings["Type"].get<FogType>();
      }
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["Color"] = Color;
      json["Start"] = Start;
      json["End"] = End;
      json["Type"] = static_cast<int>(Type);
      return json;
    }

    void
    NotifyObservers()
    {
      for (auto* observer : Observers.get())
      {
        observer->OnFogSettingsChanged();
      }
    }

    [[nodiscard]] auto
    GetFogType() const -> FogType
    {
      return Type;
    }

    void
    SetFogType(FogType type)
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
    GetStart() const -> float
    {
      return Start;
    }

    void
    SetStart(float start)
    {
      Start = start;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetEnd() const -> float
    {
      return End;
    }

    void
    SetEnd(float end)
    {
      End = end;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetDensity() const -> float
    {
      return Density;
    }

    void
    SetDensity(float density)
    {
      Density = density;
      NotifyObservers();
    }
  };
}