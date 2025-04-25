#include "pch.hpp"

#include "Launcher/SavedProjects/ISavedProjects.hpp"
#include "SavedProjectsIO.hpp"

#define SAVED_PROJECTS_FILE_NAME "savedProjects.json"
#define SAVED_PROJECTS_KEY "projects"
#define SAVED_PROJECTS_PROJECT_NAME_KEY "name"
#define SAVED_PROJECTS_PROJECT_PATH_KEY "path"
#define SAVED_PROJECTS_GRAPHICS_API_KEY "graphicsApi"
#define SAVED_PROJECTS_LAST_OPENED_KEY "lastOpened"

namespace Dwarf
{
  SavedProjectsIO::SavedProjectsIO(std::shared_ptr<IDwarfLogger> logger,
                                   std::shared_ptr<IFileHandler> fileHandler)
    : mLogger(std::move(logger))
    , mFileHandler(std::move(fileHandler))
  {
    mLogger->LogDebug(Log("SavedProjectsIO created", "SavedProjectsIO"));
  }

  SavedProjectsIO::~SavedProjectsIO()
  {
    mLogger->LogDebug(Log("SavedProjectsIO destroyed", "SavedProjectsIO"));
  }

  auto
  SavedProjectsIO::LoadSavedProjects() const -> std::vector<SavedProject>
  {
    std::vector<SavedProject> savedProjects;
    std::filesystem::path     savedProjectsPath =
      mFileHandler->GetEngineSettingsPath() / SAVED_PROJECTS_FILE_NAME;

    mLogger->LogInfo(Log(
      fmt::format("Loading project list from {}", savedProjectsPath.string()),
      "SavedProjectsIO"));

    if (mFileHandler->FileExists(savedProjectsPath))
    {
      std::string    fileContent = mFileHandler->ReadFile(savedProjectsPath);
      nlohmann::json jsonObject = nlohmann::json::parse(fileContent);

      mLogger->LogDebug(Log("Parsing project list", "SavedProjectsIO"));

      if (jsonObject.contains(SAVED_PROJECTS_KEY))
      {
        nlohmann::json projectsArray = jsonObject.at(SAVED_PROJECTS_KEY);
        for (int i = 0; i < projectsArray.size(); i++)
        {
          SavedProject projectToAdd;
          const auto&  project = projectsArray[i];

          projectToAdd.ProjectName =
            project.value(SAVED_PROJECTS_PROJECT_NAME_KEY, "");

          projectToAdd.Path =
            project.value(SAVED_PROJECTS_PROJECT_PATH_KEY, "");

          projectToAdd.LastOpened =
            project.value(SAVED_PROJECTS_LAST_OPENED_KEY, -1);

          projectToAdd.Api =
            project.value(SAVED_PROJECTS_GRAPHICS_API_KEY, GraphicsApi::None);

          savedProjects.push_back({ projectToAdd });
        }

        mLogger->LogInfo(
          Log(fmt::format("Loaded {} projects", savedProjects.size()),
              "SavedProjectsIO"));
      }
    }
    else
    {
      mLogger->LogInfo(
        Log("No projects found in project list", "SavedProjectsIO"));
    }
    return savedProjects;
  }

  void
  SavedProjectsIO::SaveSavedProjects(
    const std::vector<SavedProject>& savedProjects) const
  {
    nlohmann::json jsonObject;
    mLogger->LogInfo(Log("Saving project list", "SavedProjectsIO"));
    for (int i = 0; i < savedProjects.size(); i++)
    {
      jsonObject[SAVED_PROJECTS_KEY][i][SAVED_PROJECTS_PROJECT_NAME_KEY] =
        savedProjects[i].ProjectName;
      jsonObject[SAVED_PROJECTS_KEY][i][SAVED_PROJECTS_PROJECT_PATH_KEY] =
        savedProjects[i].Path.string();
      jsonObject[SAVED_PROJECTS_KEY][i][SAVED_PROJECTS_LAST_OPENED_KEY] =
        savedProjects[i].LastOpened;
      jsonObject[SAVED_PROJECTS_KEY][i][SAVED_PROJECTS_GRAPHICS_API_KEY] =
        savedProjects[i].Api;
    }

    std::string           fileContent = jsonObject.dump(4);
    std::filesystem::path settingsPath = mFileHandler->GetEngineSettingsPath();
    if (!mFileHandler->DirectoryExists(settingsPath))
    {
      mFileHandler->CreateDirectoryAt(settingsPath);
    }
    std::filesystem::path savedProjectsPath =
      settingsPath / "savedProjects.json";
    mFileHandler->WriteToFile(savedProjectsPath.make_preferred(), fileContent);

    mLogger->LogInfo(Log("Project list saved", "SavedProjectsIO"));
  }
}