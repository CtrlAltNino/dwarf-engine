#include "Editor/LoadedScene/LoadedScene.h"

namespace Dwarf
{
  LoadedScene::LoadedScene(std::shared_ptr<IProjectSettings> projectSettings)
    : m_ProjectSettings(projectSettings)
  {
  }

  IScene&
  LoadedScene::GetScene()
  {
    return *m_Scene;
  }

  void
  LoadedScene::SetScene(std::unique_ptr<IScene> scene)
  {
    m_Scene = std::move(scene);
    if (m_Scene->GetProperties().GetAssetID().has_value())
    {
      m_ProjectSettings->UpdateLastOpenedScene(
        m_Scene->GetProperties().GetAssetID().value());
    }
  }
}