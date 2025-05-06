#pragma once

namespace Dwarf
{
  class ISceneSettingsObserver
  {
  public:
    virtual ~ISceneSettingsObserver() = default;

    virtual void
    OnAntiAliasingSettingsChanged() = 0;

    virtual void
    OnExposureSettingsChanged() = 0;

    virtual void
    OnTonemapChanged() = 0;

    virtual void
    OnBloomSettingsChanged() = 0;

    virtual void
    OnSkyboxSettingsChanged() = 0;
  };
}