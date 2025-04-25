#include "pch.hpp"

#include "IProjectSettings.hpp"
#include "ProjectSettingsIO.hpp"

namespace Dwarf
{
  ProjectSettingsIO::ProjectSettingsIO(
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IFileHandler> fileHandler)
    : mLogger(std::move(logger))
    , mFileHandler(std::move(fileHandler))
  {
  }

  auto
  ProjectSettingsIO::LoadProjectSettings(std::filesystem::path projectPath)
    -> std::optional<ProjectSettingsData>
  {
    ProjectSettingsData   projectSettingsData;
    std::filesystem::path projectSettingsPath =
      projectPath / "projectSettings.dproj";
    mLogger->LogInfo(Log("Loading project settings from [" +
                           projectSettingsPath.string() + "]",
                         "ProjectSettings"));

    if (!mFileHandler->FileExists(projectSettingsPath))
    {
      mLogger->LogError(Log("Project settings file does not exist at [" +
                              projectSettingsPath.string() + "]",
                            "ProjectSettings"));
      return std::nullopt;
    }

    // Loading project settings from file
    nlohmann::json projectSettings =
      nlohmann::json::parse(mFileHandler->ReadFile(projectSettingsPath));

    if (projectSettings.contains(PROJECT_NAME_KEY))
    {
      projectSettingsData.ProjectName =
        projectSettings.at(PROJECT_NAME_KEY).get<std::string>();
    }
    else
    {
      mLogger->LogError(
        Log("Missing required field: projectName", "ProjectSettings"));
      return std::nullopt;
    }

    if (projectSettings.contains(GRAPHICS_API_KEY))
    {
      projectSettingsData.Api =
        projectSettings.at(GRAPHICS_API_KEY).get<GraphicsApi>();
    }
    else
    {
      mLogger->LogError(
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
      mLogger->LogError(
        Log(std::format("Missing required field: %s", LAST_OPENED_SCENE_KEY),
            "ProjectSettings"));
      projectSettingsData.LastOpenedScene =
        std::nullopt; // or some default value
    }

    // Check if layout key is present, if so, load into mData.Layout
    if (projectSettings.contains(VIEW_KEY))
    {
      projectSettingsData.SerializedView = projectSettings.at(VIEW_KEY);
    }
    else
    {
      mLogger->LogError(Log(std::format("Missing required field: %s", VIEW_KEY),
                            "ProjectSettings"));
    }

    return projectSettingsData;
  }

  void
  ProjectSettingsIO::SaveProjectSettings(
    ProjectSettingsData   projectSettingsData,
    std::filesystem::path projectPath)
  {
    nlohmann::json projectSettings = projectSettingsData.Serialize();

    mFileHandler->WriteToFile(projectPath / "projectSettings.dproj",
                              projectSettings.dump(2));
  }
}