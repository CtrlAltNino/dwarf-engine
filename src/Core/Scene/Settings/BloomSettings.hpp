#pragma once

#include "Core/Scene/Settings/ISceneSettingsObserver.hpp"
#include "Utilities/ISerializable.hpp"

namespace Dwarf
{
  struct BloomSettings : public ISerializable
  {
  private:
    std::reference_wrapper<std::vector<ISceneSettingsObserver*>> Observers;
    bool                                                         Enabled = true;
    float Threshold = 0.7F;
    float Intensity = 1.0F;
    float Radius = 1.0F;

  public:
    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    BloomSettings(std::vector<ISceneSettingsObserver*>& observers,
                  nlohmann::json                        json = "")
      : Observers(observers)
    {
      if (json.contains("Enabled"))
      {
        Enabled = json["Enabled"].get<bool>();
      }
      if (json.contains("Threshold"))
      {
        Threshold = json["Threshold"].get<float>();
      }
      if (json.contains("Intensity"))
      {
        Intensity = json["Intensity"].get<float>();
      }
      if (json.contains("Radius"))
      {
        Radius = json["Radius"].get<float>();
      }
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["Enabled"] = Enabled;
      json["Threshold"] = Threshold;
      json["Intensity"] = Intensity;
      json["Radius"] = Radius;
      return json;
    }

    void
    NotifyObservers()
    {
      for (auto* observer : Observers.get())
      {
        observer->OnBloomSettingsChanged();
      }
    }

    [[nodiscard]] auto
    GetEnabled() const -> bool
    {
      return Enabled;
    }

    void
    SetEnabled(bool enabled)
    {
      Enabled = enabled;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetThreshold() const -> float
    {
      return Threshold;
    }

    void
    SetThreshold(float threshold)
    {
      Threshold = threshold;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetIntensity() const -> float
    {
      return Intensity;
    }

    void
    SetIntensity(float intensity)
    {
      Intensity = intensity;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetRadius() const -> float
    {
      return Radius;
    }

    void
    SetRadius(float radius)
    {
      Radius = radius;
      NotifyObservers();
    }
  };
}