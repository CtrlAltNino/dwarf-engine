#include "pch.hpp"

#include "ShadowMapper.hpp"
#include "ShadowMapperFactory.hpp"

namespace Dwarf
{
  ShadowMapperFactory::ShadowMapperFactory(
    std::shared_ptr<ILoadedScene> loadedScene)
    : mLoadedScene(std::move(loadedScene))
  {
  }
  auto
  ShadowMapperFactory::Create() -> std::unique_ptr<IShadowMapper>
  {
    return std::make_unique<ShadowMapper>(mLoadedScene);
  }
}