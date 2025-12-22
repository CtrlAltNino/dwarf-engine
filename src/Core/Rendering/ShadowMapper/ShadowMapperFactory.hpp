#pragma once

#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "IShadowMapperFactory.hpp"

namespace Dwarf
{
  class ShadowMapperFactory : public IShadowMapperFactory
  {
  private:
    std::shared_ptr<ILoadedScene> mLoadedScene;

  public:
    ShadowMapperFactory(std::shared_ptr<ILoadedScene> loadedScene);

    ~ShadowMapperFactory() override = default;

    auto
    Create() -> std::unique_ptr<IShadowMapper> override;
  };
}