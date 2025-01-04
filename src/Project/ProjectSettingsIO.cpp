#include "ProjectSettingsIO.h"
#include "IProjectSettings.h"
#include <optional>

#define GRAPHICS_API_KEY "graphicsApi"
#define LAST_OPENED_SCENE_KEY "lastOpenedScene"
#define PROJECT_NAME_KEY "projectName"
#define VIEW_KEY "view"

namespace Dwarf
{
  ProjectSettingsIO::ProjectSettingsIO(
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IFileHandler> fileHandler)
    : m_Logger(logger)
    , m_FileHandler(fileHandler)
  {
  }

  std::optional<ProjectSettingsData>
  ProjectSettingsIO::LoadProjectSettings(std::filesystem::path projectPath)
  {
    ProjectSettingsData   projectSettingsData;
    std::filesystem::path projectSettingsPath =
      projectPath / "projectSettings.dproj";
    m_Logger->LogInfo(Log("Loading project settings from [" +
                            projectSettingsPath.string() + "]",
                          "ProjectSettings"));

    if (!m_FileHandler->FileExists(projectSettingsPath))
    {
      m_Logger->LogError(Log("Project settings file does not exist at [" +
                               projectSettingsPath.string() + "]",
                             "ProjectSettings"));
      return std::nullopt;
    }

    // Loading project settings from file
    nlohmann::json projectSettings =
      nlohmann::json::parse(m_FileHandler->ReadFile(projectSettingsPath));

    if (projectSettings.contains(PROJECT_NAME_KEY))
    {
      projectSettingsData.ProjectName =
        projectSettings.at(PROJECT_NAME_KEY).get<std::string>();
    }
    else
    {
      m_Logger->LogError(
        Log("Missing required field: projectName", "ProjectSettings"));
      return std::nullopt;
    }

    if (projectSettings.contains(GRAPHICS_API_KEY))
    {
      projectSettingsData.GraphicsApi =
        projectSettings.at(GRAPHICS_API_KEY).get<GraphicsApi>();
    }
    else
    {
      m_Logger->LogError(
        Log(std::format("Missing required field: %s", GRAPHICS_API_KEY),
            "ProjectSettings"));
      return std::nullopt;
    }

    if (projectSettings.contains(LAST_OPENED_SCENE_KEY) &&
        !projectSettings.at(LAST_OPENED_SCENE_KEY).get<std::string>().empty())
    {
      projectSettingsData.LastOpenedScene =
        UUID(projectSettings.at(LAST_OPENED_SCENE_KEY).get<std::string>());
    }
    else
    {
      m_Logger->LogError(
        Log(std::format("Missing required field: %s", LAST_OPENED_SCENE_KEY),
            "ProjectSettings"));
      projectSettingsData.LastOpenedScene =
        std::nullopt; // or some default value
    }

    // Check if layout key is present, if so, load into m_Data.Layout
    if (projectSettings.contains(VIEW_KEY))
    {
      projectSettingsData.SerializedView = projectSettings.at(VIEW_KEY);
    }
    else
    {
      m_Logger->LogError(
        Log(std::format("Missing required field: %s", VIEW_KEY),
            "ProjectSettings"));
    }

    return projectSettingsData;
  }

  void
  ProjectSettingsIO::SaveProjectSettings(
    ProjectSettingsData   projectSettingsData,
    std::filesystem::path projectPath)
  {
    m_Logger->LogInfo(Log("Saving project settings...", "ProjectSettings"));
    nlohmann::json projectSettings;
    projectSettings[PROJECT_NAME_KEY] = projectSettingsData.ProjectName;
    projectSettings[GRAPHICS_API_KEY] = projectSettingsData.GraphicsApi;
    projectSettings[LAST_OPENED_SCENE_KEY] =
      projectSettingsData.LastOpenedScene.has_value()
        ? projectSettingsData.LastOpenedScene->ToString()
        : "";
    projectSettings[VIEW_KEY] = projectSettingsData.SerializedView;

    m_FileHandler->WriteToFile(projectPath / "projectSettings.dproj",
                               projectSettings.dump(2));
  }
}