#pragma once

#include "Core/Rendering/ExposureTypes.hpp"
#include "ISceneSettingsObserver.hpp"
#include "Utilities/ISerializable.hpp"

namespace Dwarf
{
  struct ExposureSettings : public ISerializable
  {
  private:
    ExposureType Type = ExposureType::Manual;
    float        Exposure = 1.5F;
    std::reference_wrapper<std::vector<ISceneSettingsObserver*>> Observers;

  public:
    /// @brief Constructor.
    /// @param serializedSettings The serialized settings.
    ExposureSettings(std::vector<ISceneSettingsObserver*>& observers,
                     nlohmann::json                        json = "")
      : Observers(observers)
    {
      if (json.contains("Type"))
      {
        Type = json["Type"].get<ExposureType>();
      }
      if (json.contains("Exposure"))
      {
        Exposure = json["Exposure"].get<float>();
      }
    }

    /// @copydoc ISerializable::Serialize
    auto
    Serialize() -> nlohmann::json override
    {
      nlohmann::json json;
      json["Type"] = Type;
      json["Exposure"] = Exposure;
      return json;
    }

    void
    NotifyObservers()
    {
      for (auto* observer : Observers.get())
      {
        observer->OnExposureSettingsChanged();
      }
    }

    [[nodiscard]] auto
    GetExposureType() const -> ExposureType
    {
      return Type;
    }

    void
    SetExposureType(ExposureType type)
    {
      Type = type;
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
  };
}