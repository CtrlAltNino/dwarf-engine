#include "Editor/LoadedScene/LoadedScene.h"
#include <fmt/format.h>

namespace Dwarf
{
  LoadedScene::LoadedScene(std::shared_ptr<IDwarfLogger>     logger,
                           std::shared_ptr<IProjectSettings> projectSettings)
    : m_Logger(logger)
    , m_ProjectSettings(projectSettings)
  {
    m_Logger->LogDebug(Log("LoadedScene created", "LoadedScene"));
  }

  LoadedScene::~LoadedScene()
  {
    m_Logger->LogDebug(Log("LoadedScene destroyed", "LoadedScene"));
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
    if (scene)
    {
      m_Logger->LogDebug(Log(
        fmt::format("Setting scene: {}", m_Scene->GetProperties().GetName()),
        "LoadedScene"));
      if (m_Scene->GetProperties().GetAssetID().has_value())
      {
        m_ProjectSettings->UpdateLastOpenedScene(
          m_Scene->GetProperties().GetAssetID().value());
      }
    }
    else
    {
      m_Logger->LogDebug(Log("Unloading scene", "LoadedScene"));
    }
  }
}