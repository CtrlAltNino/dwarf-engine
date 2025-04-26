#pragma once

#include "Core/Rendering/AntiAliasingTypes.hpp"
#include "ISceneSettingsObserver.hpp"
#include "Utilities/ISerializable.hpp"

namespace Dwarf
{

  struct AntiAliasingSettings : public ISerializable
  {
  private:
    std::reference_wrapper<std::vector<ISceneSettingsObserver*>> Observers;
    AntiAliasingMethod Type = AntiAliasingMethod::None;
    uint8_t            MsaaSamples = 1;

  public:
    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    AntiAliasingSettings(std::vector<ISceneSettingsObserver*>& observers,
                         nlohmann::json                        json = "")
      : Observers(observers)
    {
      if (json.contains("Type"))
      {
        Type = json["Type"].get<AntiAliasingMethod>();
      }
      if (json.contains("MsaaSamples"))
      {
        MsaaSamples = json["MsaaSamples"].get<uint8_t>();
      }
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["Type"] = Type;
      json["MsaaSamples"] = MsaaSamples;
      return json;
    }

    void
    NotifyObservers()
    {
      for (auto* observer : Observers.get())
      {
        observer->OnAntiAliasingSettingsChanged();
      }
    }

    [[nodiscard]] auto
    GetAntiAliasingMethod() const -> AntiAliasingMethod
    {
      return Type;
    }

    void
    SetAntiAliasingMethod(AntiAliasingMethod type)
    {
      Type = type;
      NotifyObservers();
    }

    [[nodiscard]] auto
    GetSamples() const -> uint8_t
    {
      return MsaaSamples;
    }

    void
    SetSamples(uint8_t samples)
    {
      MsaaSamples = samples;
      NotifyObservers();
    }
  };
}