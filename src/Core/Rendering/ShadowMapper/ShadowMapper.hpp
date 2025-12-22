#pragma once

#include "Core/Scene/ISceneObserver.hpp"
#include "Core/Scene/Settings/ISceneSettingsObserver.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "IShadowMapper.hpp"

namespace Dwarf
{
  class ShadowMapper
    : public IShadowMapper
    , public ISceneObserver
    , public ISceneSettingsObserver
  {
  private:
    ShadowFrameData mShadowFrameData;

    std::shared_ptr<ILoadedScene> mLoadedScene;

  public:
    ShadowMapper(std::shared_ptr<ILoadedScene> loadedScene);

    void
    Update() override;

    [[nodiscard]] auto
    GetFrameData() const -> const ShadowFrameData override;

    void
    OnEntityCreated() override {};

    void
    OnEntityDeleted() override {};

    void
    OnAntiAliasingSettingsChanged() override {};

    void
    OnExposureSettingsChanged() override {};

    void
    OnTonemapChanged() override {};

    void
    OnBloomSettingsChanged() override {};

    void
    OnSkyboxSettingsChanged() override {};

    void
    OnAmbientSettingsChanged() override {};

    void
    OnFogSettingsChanged() override {};

    void
    OnShadowSettingsChanged() override;
  };
}