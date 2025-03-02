#pragma once

#include "Editor/LoadedScene/ILoadedScene.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettings.h"

namespace Dwarf
{
  class LoadedScene : public ILoadedScene
  {
  private:
    std::shared_ptr<IDwarfLogger>     m_Logger;
    std::unique_ptr<IScene>           m_Scene;
    std::shared_ptr<IProjectSettings> m_ProjectSettings;

  public:
    LoadedScene(std::shared_ptr<IDwarfLogger>     logger,
                std::shared_ptr<IProjectSettings> projectSettings);
    ~LoadedScene() override;

    IScene&
    GetScene() override;

    void
    SetScene(std::unique_ptr<IScene> scene) override;
  };
}