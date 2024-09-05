#include "ProjectSettings.h"
#include "Utilities/FileHandler.h"

namespace Dwarf
{
#define GRAPHICS_API_KEY "graphicsApi"
#define LAST_OPENED_SCENE_KEY "lastOpenedScene"
#define PROJECT_LAST_OPENED_DATE_KEY "projectLastOpenedDate"
#define PROJECT_NAME_KEY "projectName"

  ProjectSettings::ProjectSettings(ProjectPath                   path,
                                   std::shared_ptr<IDwarfLogger> logger)
    : m_Logger(logger)
    , m_Path(path)
  {
    Load();
  }

  void
  ProjectSettings::Load()
  {
    m_Logger->LogInfo(
      Log("Loading project settings from [" + m_Path.t.string() + "]",
          "ProjectSettings"));

    if (!FileHandler::FileExists(m_Path.t))
    {
      m_Logger->LogError(Log("Project settings file does not exist at [" +
                               m_Path.t.string() + "]",
                             "ProjectSettings"));
      return;
    }

    // Loading project settings from file
    nlohmann::json projectSettings =
      nlohmann::json::parse(FileHandler::ReadFile(m_Path.t));

    if (projectSettings.contains(PROJECT_NAME_KEY))
    {
      m_Name = projectSettings.at(PROJECT_NAME_KEY).get<std::string>();
    }
    else
    {
      m_Logger->LogError(
        Log("Missing required field: projectName", "ProjectSettings"));
      m_LoadStatus.addMissingField(PROJECT_NAME_KEY);
    }

    if (projectSettings.contains(GRAPHICS_API_KEY))
    {
      m_GraphicsApi = projectSettings.at(GRAPHICS_API_KEY).get<GraphicsApi>();
    }
    else
    {
      m_Logger->LogError(
        Log(std::format("Missing required field: %s", GRAPHICS_API_KEY),
            "ProjectSettings"));
      m_LoadStatus.addMissingField(GRAPHICS_API_KEY);
    }

    if (projectSettings.contains(LAST_OPENED_SCENE_KEY))
    {
      m_LastOpenedScene = std::make_shared<UUID>(
        projectSettings.at(LAST_OPENED_SCENE_KEY).get<std::string>());
    }
    else
    {
      m_Logger->LogError(
        Log(std::format("Missing required field: %s", LAST_OPENED_SCENE_KEY),
            "ProjectSettings"));
      m_LastOpenedScene = nullptr; // or some default value
    }

    if (projectSettings.contains(PROJECT_LAST_OPENED_DATE_KEY))
    {
      m_LastOpenedTimeStamp =
        projectSettings.at(PROJECT_LAST_OPENED_DATE_KEY).get<time_t>();
    }
    else
    {
      m_Logger->LogError(Log(
        std::format("Missing required field: %s", PROJECT_LAST_OPENED_DATE_KEY),
        "ProjectSettings"));
      m_LastOpenedTimeStamp = 0; // or some default value
    }
  }

  void
  ProjectSettings::Save()
  {
    nlohmann::json projectSettings;
    projectSettings[PROJECT_NAME_KEY] = m_Name;
    projectSettings[GRAPHICS_API_KEY] = m_GraphicsApi;
    projectSettings[LAST_OPENED_SCENE_KEY] =
      m_LastOpenedScene == nullptr ? "" : m_LastOpenedScene->ToString();
    projectSettings[PROJECT_LAST_OPENED_DATE_KEY] = m_LastOpenedTimeStamp;

    FileHandler::WriteToFile(m_Path.t, projectSettings.dump(2));
  }

  void
  ProjectSettings::SetProjectName(const std::string& projectName)
  {
    m_Name = projectName;
  }

  std::string
  ProjectSettings::GetProjectName() const
  {
    return m_Name;
  }

  void
  ProjectSettings::SetGraphicsApi(const GraphicsApi& graphicsAPI)
  {
    m_GraphicsApi = graphicsAPI;
  }

  const GraphicsApi&
  ProjectSettings::GetGraphicsApi() const
  {
    return m_GraphicsApi;
  }

  void
  ProjectSettings::SetLastOpenedScene(const std::shared_ptr<UUID>& sceneGUID)
  {
    m_LastOpenedScene = sceneGUID;
  }

  const std::shared_ptr<UUID>&
  ProjectSettings::GetLastOpenedScene() const
  {
    return m_LastOpenedScene;
  }

  void
  ProjectSettings::SetLastOpenedTimeStamp(const time_t& timeStamp)
  {
    m_LastOpenedTimeStamp = timeStamp;
  }

  const time_t&
  ProjectSettings::GetLastOpenedTimeStamp() const
  {
    return m_LastOpenedTimeStamp;
  }
}