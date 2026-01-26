#pragma once

#include "Core/Rendering/Framebuffer/IFramebufferFactory.hpp"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.hpp"
#include "Core/Rendering/Texture/ITextureFactory.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "IShadowMapperFactory.hpp"

namespace Dwarf
{
  class ShadowMapperFactory : public IShadowMapperFactory
  {
  private:
    std::shared_ptr<ILoadedScene>        mLoadedScene;
    std::shared_ptr<IFramebufferFactory> mFramebufferFactory;
    std::shared_ptr<ITextureFactory>     mTextureFactory;
    std::shared_ptr<IShaderRegistry>     mShaderRegistry;

  public:
    ShadowMapperFactory(std::shared_ptr<ILoadedScene>        loadedScene,
                        std::shared_ptr<IFramebufferFactory> framebufferFactory,
                        std::shared_ptr<ITextureFactory>     textureFactory,
                        std::shared_ptr<IShaderRegistry>     shaderRegistry);

    ~ShadowMapperFactory() override = default;

    auto
    Create() -> std::unique_ptr<IShadowMapper> override;
  };
}