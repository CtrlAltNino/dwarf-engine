#include "pch.hpp"

#include "Core/Rendering/LightSystem/LightSystem.hpp"
#include "LightSystemFactory.hpp"

namespace Dwarf
{
  LightSystemFactory::LightSystemFactory(
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<ILoadedScene> loadedScene)
    : mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
  {
  }

  LightSystemFactory::~LightSystemFactory() {}

  auto
  LightSystemFactory::Create() -> std::unique_ptr<ILightSystem>
  {
    return std::make_unique<LightSystem>(mLogger, mLoadedScene);
  }
}