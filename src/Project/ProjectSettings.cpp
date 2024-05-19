#include "ProjectSettings.h"
#include "Core/Asset/AssetComponents.h"
#include "Core/Asset/AssetReference.h"

namespace Dwarf
{
#define GRAPHICS_API_KEY "graphicsApi"
#define LAST_OPENED_SCENE_KEY "lastOpenedScene"
#define PROJECT_LAST_OPENED_DATE_KEY "projectLastOpenedDate"
#define PROJECT_NAME_KEY "projectName"

  ProjectSettings::ProjectSettings(ProjectPath                          path,
                                   const std::shared_ptr<IDwarfLogger>& logger)
    : m_Logger(logger)
  {
    m_Data.projectPath = path;
    Load();
  }

  void
  ProjectSettings::Load()
  {
    m_Logger->LogInfo("Loading project settings from [" +
                      m_Data.projectPath.string() + "]");

    if (!FileHandler::CheckIfFileExists(m_Data.projectPath))
    {
      m_Logger->LogError("Project settings file does not exist at [" +
                         m_Data.projectPath.string() + "]");
      return;
    }

    // Loading project settings from file
    nlohmann::json projectSettings =
      nlohmann::json::parse(FileHandler::ReadFile(m_Data.projectPath));

    if (projectSettings.contains(PROJECT_NAME_KEY))
    {
      m_Data.projectName =
        projectSettings.at(PROJECT_NAME_KEY).get<std::string>();
    }
    else
    {
      m_Logger->LogError("Missing required field: projectName");
      m_LoadStatus.addMissingField(PROJECT_NAME_KEY);
    }

    if (projectSettings.contains(GRAPHICS_API_KEY))
    {
      m_Data.graphicsAPI =
        projectSettings.at(GRAPHICS_API_KEY).get<std::string>();
    }
    else
    {
      m_Logger->LogError(
        std::format("Missing required field: %s", GRAPHICS_API_KEY));
      m_LoadStatus.addMissingField(GRAPHICS_API_KEY);
    }

    if (projectSettings.contains(LAST_OPENED_SCENE_KEY))
    {
      m_Data.lastOpenedSceneGUID =
        projectSettings.at(LAST_OPENED_SCENE_KEY).get<std::string>();
    }
    else
    {
      m_Logger->LogError(
        std::format("Missing required field: %s", LAST_OPENED_SCENE_KEY));
      m_Data.lastOpenedSceneGUID = ""; // or some default value
    }

    if (projectSettings.contains(PROJECT_LAST_OPENED_DATE_KEY))
    {
      m_Data.lastOpenedTimeStamp =
        projectSettings.at(PROJECT_LAST_OPENED_DATE_KEY).get<std::string>();
    }
    else
    {
      m_Logger->LogError(std::format("Missing required field: %s",
                                     PROJECT_LAST_OPENED_DATE_KEY));
      m_Data.lastOpenedTimeStamp = ""; // or some default value
    }
  }

  void
  ProjectSettings::Save()
  {
    nlohmann::json projectSettings;
    projectSettings[PROJECT_NAME_KEY] = m_Data.projectName;
    projectSettings[GRAPHICS_API_KEY] = m_Data.graphicsAPI;
    projectSettings[LAST_OPENED_SCENE_KEY] = m_Data.lastOpenedSceneGUID;
    projectSettings[PROJECT_LAST_OPENED_DATE_KEY] = m_Data.lastOpenedTimeStamp;

    FileHandler::WriteToFile(m_Data.projectPath, projectSettings.dump(2));
  }

  void
  ProjectSettings::SetProjectName(const std::string& projectName)
  {
    m_Data.projectName = projectName;
  }

  std::string
  ProjectSettings::GetProjectName() const
  {
    return m_Data.projectName;
  }

  void
  ProjectSettings::SetGraphicsApi(const GraphicsApi& graphicsAPI)
  {
    m_Data.graphicsAPI = graphicsAPI;
  }

  const GraphicsApi&
  ProjectSettings::GetGraphicsApi() const
  {
    return m_Data.graphicsAPI;
  }

  void
  ProjectSettings::SetLastOpenedScene(
    const AssetReference<SceneAsset>& sceneGUID)
  {
    m_Data.lastOpenedSceneGUID = sceneGUID;
  }

  const AssetReference<SceneAsset>&
  ProjectSettings::GetLastOpenedScene() const
  {
    return m_Data.lastOpenedSceneGUID;
  }

  void
  ProjectSettings::SetLastOpenedTimeStamp(const time_t& timeStamp)
  {
    m_Data.lastOpenedTimeStamp = timeStamp;
  }

  const time_t&
  ProjectSettings::GetLastOpenedTimeStamp() const
  {
    return m_Data.lastOpenedTimeStamp;
  }
}