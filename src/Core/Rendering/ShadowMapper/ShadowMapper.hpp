#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.hpp"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.hpp"
#include "Core/Scene/ISceneObserver.hpp"
#include "Core/Scene/Settings/ISceneSettingsObserver.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "IShadowMapper.hpp"

namespace Dwarf
{
  constexpr uint32_t shadow_map_size = 2048;

  class ShadowMapper
    : public IShadowMapper
    , public ISceneObserver
    , public ISceneSettingsObserver
  {
  private:
    ShadowFrameData mShadowFrameData;

    std::shared_ptr<ILoadedScene>        mLoadedScene;
    std::shared_ptr<IFramebufferFactory> mFramebufferFactory;

    std::shared_ptr<IFramebuffer> mFramebuffer;

  public:
    ShadowMapper(std::shared_ptr<ILoadedScene>        loadedScene,
                 std::shared_ptr<IFramebufferFactory> framebufferFactory);

    void
    Update(LightData& lightData) override;

    [[nodiscard]] auto
    GetFrameData() const -> const ShadowFrameData override;

    void
    Bind() const override;

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