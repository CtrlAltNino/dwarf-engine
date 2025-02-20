#include "ProjectSettings.h"
#include "IProjectSettings.h"

namespace Dwarf
{
  ProjectSettings::ProjectSettings(
    ProjectPath                         path,
    std::shared_ptr<IDwarfLogger>       logger,
    std::shared_ptr<IFileHandler>       fileHandler,
    std::shared_ptr<IProjectSettingsIO> projectSettingsIO)
    : m_Logger(logger)
    , m_ProjectPath(path.t)
    , m_FileHandler(fileHandler)
    , m_ProjectSettingsIO(projectSettingsIO)
  {
    std::optional<ProjectSettingsData> projectSettingsData =
      m_ProjectSettingsIO->LoadProjectSettings(m_ProjectPath);

    if (projectSettingsData)
    {
      m_Data = projectSettingsData.value();
    }
  }

  void
  ProjectSettings::Save()
  {
    m_ProjectSettingsIO->SaveProjectSettings(m_Data, m_ProjectPath);
  }

  void
  ProjectSettings::UpdateProjectName(const std::string& projectName)
  {
    m_Data.ProjectName = projectName;
  }

  std::string
  ProjectSettings::GetProjectName() const
  {
    return m_Data.ProjectName;
  }

  void
  ProjectSettings::UpdateGraphicsApi(const GraphicsApi& graphicsAPI)
  {
    m_Data.GraphicsApi = graphicsAPI;
  }

  const GraphicsApi&
  ProjectSettings::GetGraphicsApi() const
  {
    return m_Data.GraphicsApi;
  }

  void
  ProjectSettings::UpdateLastOpenedScene(const UUID& sceneGUID)
  {
    m_Data.LastOpenedScene = sceneGUID;
  }

  const std::optional<UUID>&
  ProjectSettings::GetLastOpenedScene() const
  {
    return m_Data.LastOpenedScene;
  }

  void
  ProjectSettings::UpdateSerializedView(const nlohmann::json& serializedView)
  {
    m_Data.SerializedView = serializedView;
  }

  nlohmann::json
  ProjectSettings::GetSerializedView() const
  {
    return m_Data.SerializedView;
  }
}