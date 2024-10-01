#include "ProjectSettings.h"

namespace Dwarf
{
  ProjectSettings::ProjectSettings(ProjectPath                   path,
                                   std::shared_ptr<IDwarfLogger> logger,
                                   std::shared_ptr<IFileHandler> fileHandler)
    : m_Logger(logger)
    , m_ProjectSettingsPath(path.t / "projectSettings.dproj")
    , m_FileHandler(fileHandler)
  {
    Load();
  }

  void
  ProjectSettings::Load()
  {
    m_Logger->LogInfo(Log("Loading project settings from [" +
                            m_ProjectSettingsPath.string() + "]",
                          "ProjectSettings"));

    if (!m_FileHandler->FileExists(m_ProjectSettingsPath))
    {
      m_Logger->LogError(Log("Project settings file does not exist at [" +
                               m_ProjectSettingsPath.string() + "]",
                             "ProjectSettings"));
      return;
    }

    // Loading project settings from file
    nlohmann::json projectSettings =
      nlohmann::json::parse(m_FileHandler->ReadFile(m_ProjectSettingsPath));

    if (projectSettings.contains(PROJECT_NAME_KEY))
    {
      m_Data.ProjectName =
        projectSettings.at(PROJECT_NAME_KEY).get<std::string>();
    }
    else
    {
      m_Logger->LogError(
        Log("Missing required field: projectName", "ProjectSettings"));
      m_LoadStatus.addMissingField(PROJECT_NAME_KEY);
    }

    if (projectSettings.contains(GRAPHICS_API_KEY))
    {
      m_Data.GraphicsApi =
        projectSettings.at(GRAPHICS_API_KEY).get<GraphicsApi>();
    }
    else
    {
      m_Logger->LogError(
        Log(std::format("Missing required field: %s", GRAPHICS_API_KEY),
            "ProjectSettings"));
      m_LoadStatus.addMissingField(GRAPHICS_API_KEY);
    }

    if (projectSettings.contains(LAST_OPENED_SCENE_KEY) &&
        !projectSettings.at(LAST_OPENED_SCENE_KEY).get<std::string>().empty())
    {
      m_Data.LastOpenedScene =
        UUID(projectSettings.at(LAST_OPENED_SCENE_KEY).get<std::string>());
    }
    else
    {
      m_Logger->LogError(
        Log(std::format("Missing required field: %s", LAST_OPENED_SCENE_KEY),
            "ProjectSettings"));
      m_Data.LastOpenedScene = std::nullopt; // or some default value
    }

    if (projectSettings.contains(PROJECT_LAST_OPENED_DATE_KEY))
    {
      m_Data.LastOpenedDate =
        projectSettings.at(PROJECT_LAST_OPENED_DATE_KEY).get<time_t>();
    }
    else
    {
      m_Logger->LogError(Log(
        std::format("Missing required field: %s", PROJECT_LAST_OPENED_DATE_KEY),
        "ProjectSettings"));
      m_Data.LastOpenedDate = 0; // or some default value
    }

    // Check if layout key is present, if so, load into m_Data.Layout
    if (projectSettings.contains(VIEW_KEY))
    {
      m_Data.SerializedView = projectSettings.at(VIEW_KEY);
    }
    else
    {
      m_Logger->LogError(
        Log(std::format("Missing required field: %s", VIEW_KEY),
            "ProjectSettings"));
      m_LoadStatus.addMissingField(VIEW_KEY);
    }
  }

  void
  ProjectSettings::Save()
  {
    m_Logger->LogInfo(Log("Saving project settings...", "ProjectSettings"));
    nlohmann::json projectSettings;
    projectSettings[PROJECT_NAME_KEY] = m_Data.ProjectName;
    projectSettings[GRAPHICS_API_KEY] = m_Data.GraphicsApi;
    projectSettings[LAST_OPENED_SCENE_KEY] =
      m_Data.LastOpenedScene.has_value() ? m_Data.LastOpenedScene->ToString()
                                         : "";
    projectSettings[PROJECT_LAST_OPENED_DATE_KEY] = m_Data.LastOpenedDate;
    projectSettings[VIEW_KEY] = m_Data.SerializedView;

    m_FileHandler->WriteToFile(m_ProjectSettingsPath, projectSettings.dump(2));
  }

  void
  ProjectSettings::UpdateProjectName(const std::string& projectName)
  {
    m_Data.ProjectName = projectName;
    Save();
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
    Save();
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
    Save();
  }

  const std::optional<UUID>&
  ProjectSettings::GetLastOpenedScene() const
  {
    return m_Data.LastOpenedScene;
  }

  void
  ProjectSettings::UpdateLastOpenedTimeStamp(const time_t& timeStamp)
  {
    m_Data.LastOpenedDate = timeStamp;
    Save();
  }

  const time_t&
  ProjectSettings::GetLastOpenedTimeStamp() const
  {
    return m_Data.LastOpenedDate;
  }

  void
  ProjectSettings::UpdateSerializedView(const nlohmann::json& serializedView)
  {
    m_Data.SerializedView = serializedView;
    Save();
  }

  nlohmann::json
  ProjectSettings::GetSerializedView() const
  {
    return m_Data.SerializedView;
  }
}