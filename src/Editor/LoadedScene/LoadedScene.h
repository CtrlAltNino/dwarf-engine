#pragma once

#include "Editor/LoadedScene/ILoadedScene.h"
#include "Project/IProjectSettings.h"

namespace Dwarf
{
  class LoadedScene : public ILoadedScene
  {
  private:
    std::unique_ptr<IScene>           m_Scene;
    std::shared_ptr<IProjectSettings> m_ProjectSettings;

  public:
    LoadedScene(std::shared_ptr<IProjectSettings> projectSettings);
    ~LoadedScene() override = default;

    std::unique_ptr<IScene>&
    GetScene() override;

    void
    SetScene(std::unique_ptr<IScene>& scene) override;
  };
}