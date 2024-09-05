#pragma once

#include "Editor/LoadedScene/ILoadedScene.h"
#include "Project/IProjectSettings.h"

namespace Dwarf
{
  class LoadedScene : public ILoadedScene
  {
  private:
    std::shared_ptr<IScene>           m_Scene;
    std::shared_ptr<IProjectSettings> m_ProjectSettings;

  public:
    LoadedScene(std::shared_ptr<IProjectSettings> projectSettings);
    ~LoadedScene() override = default;

    std::shared_ptr<IScene>
    GetScene() const override;

    void
    SetScene(std::shared_ptr<IScene> scene) override;
  };
}