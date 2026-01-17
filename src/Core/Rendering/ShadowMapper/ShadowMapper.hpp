#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.hpp"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.hpp"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.hpp"
#include "Core/Scene/ISceneObserver.hpp"
#include "Core/Scene/Settings/ISceneSettingsObserver.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "Editor/LoadedScene/ILoadedSceneObserver.h"
#include "IShadowMapper.hpp"

namespace Dwarf
{
  constexpr uint32_t shadow_map_size = 2048;

  class ShadowMapper
    : public IShadowMapper
    , public ILoadedSceneObserver
    , public ISceneSettingsObserver
  {
  private:
    ShadowFrameData mShadowFrameData;

    std::shared_ptr<ILoadedScene>        mLoadedScene;
    std::shared_ptr<IFramebufferFactory> mFramebufferFactory;
    std::shared_ptr<IShaderRegistry>     mShaderRegistry;
    std::shared_ptr<IFramebuffer>        mFramebuffer;

    auto
    BuildDirectionalLightVP(const glm::vec3& direction) -> glm::mat4;

  public:
    ShadowMapper(std::shared_ptr<ILoadedScene>        loadedScene,
                 std::shared_ptr<IFramebufferFactory> framebufferFactory,
                 std::shared_ptr<IShaderRegistry>     shaderRegistry);

    void
    Update(LightData& lightData) override;

    [[nodiscard]] auto
    GetFrameData() const -> const ShadowFrameData override;

    void
    Bind() const override;

    void
    OnSceneLoad() override;

    void
    OnSceneUnload() override;

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