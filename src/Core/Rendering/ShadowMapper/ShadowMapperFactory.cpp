#include "pch.hpp"

#include "ShadowMapper.hpp"
#include "ShadowMapperFactory.hpp"

namespace Dwarf
{
  ShadowMapperFactory::ShadowMapperFactory(
    std::shared_ptr<ILoadedScene>        loadedScene,
    std::shared_ptr<IFramebufferFactory> framebufferFactory,
    std::shared_ptr<ITextureFactory>     textureFactory,
    std::shared_ptr<IShaderRegistry>     shaderRegistry)
    : mLoadedScene(std::move(loadedScene))
    , mFramebufferFactory(std::move(framebufferFactory))
    , mTextureFactory(std::move(textureFactory))
    , mShaderRegistry(std::move(shaderRegistry))
  {
  }
  auto
  ShadowMapperFactory::Create() -> std::unique_ptr<IShadowMapper>
  {
    return std::make_unique<ShadowMapper>(
      mLoadedScene, mFramebufferFactory, mTextureFactory, mShaderRegistry);
  }
}