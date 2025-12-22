#pragma once

#include "Core/Rendering/LightSystem/ILightSystem.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "ILightSystemFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class LightSystemFactory : public ILightSystemFactory
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<ILoadedScene> mLoadedScene;

  public:
    LightSystemFactory(std::shared_ptr<IDwarfLogger> logger,
                       std::shared_ptr<ILoadedScene> loadedScene);
    ~LightSystemFactory() override;

    auto
    Create() -> std::unique_ptr<ILightSystem> override;
  };
}