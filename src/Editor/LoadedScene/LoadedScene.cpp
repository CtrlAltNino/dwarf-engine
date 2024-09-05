#include "Editor/LoadedScene/LoadedScene.h"

namespace Dwarf
{
  LoadedScene::LoadedScene(std::shared_ptr<IProjectSettings> projectSettings)
    : m_ProjectSettings(projectSettings)
  {
  }

  std::shared_ptr<IScene>
  LoadedScene::GetScene() const
  {
    return m_Scene;
  }

  void
  LoadedScene::SetScene(std::shared_ptr<IScene> scene)
  {
    m_Scene = scene;
  }
}