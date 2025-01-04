#include "SavedProjectsIO.h"
#include "Launcher/SavedProjects/ISavedProjects.h"

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
    : m_Logger(logger)
    , m_FileHandler(fileHandler)
  {
    m_Logger->LogDebug(Log("SavedProjectsIO created", "SavedProjectsIO"));
  }

  SavedProjectsIO::~SavedProjectsIO()
  {
    m_Logger->LogDebug(Log("SavedProjectsIO destroyed", "SavedProjectsIO"));
  }

  std::vector<SavedProject>
  SavedProjectsIO::LoadSavedProjects() const
  {
    std::vector<SavedProject> savedProjects;
    std::filesystem::path     savedProjectsPath =
      m_FileHandler->GetEngineSettingsPath() / SAVED_PROJECTS_FILE_NAME;

    m_Logger->LogInfo(Log(
      fmt::format("Loading project list from {}", savedProjectsPath.string()),
      "SavedProjectsIO"));
    std::string fileContent = m_FileHandler->ReadFile(savedProjectsPath);

    if (!fileContent.empty())
    {
      nlohmann::json jsonObject = nlohmann::json::parse(fileContent);

      m_Logger->LogDebug(Log("Parsing project list", "SavedProjectsIO"));

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

          projectToAdd.GraphicsApi =
            project.value(SAVED_PROJECTS_GRAPHICS_API_KEY, GraphicsApi::None);

          savedProjects.push_back({ projectToAdd });
        }

        m_Logger->LogInfo(
          Log(fmt::format("Loaded {} projects", savedProjects.size()),
              "SavedProjectsIO"));
      }
    }
    else
    {
      m_Logger->LogInfo(
        Log("No projects found in project list", "SavedProjectsIO"));
    }
    return savedProjects;
  }

  void
  SavedProjectsIO::SaveSavedProjects(
    const std::vector<SavedProject>& savedProjects) const
  {
    nlohmann::json jsonObject;
    m_Logger->LogInfo(Log("Saving project list", "SavedProjectsIO"));
    for (int i = 0; i < savedProjects.size(); i++)
    {
      jsonObject[SAVED_PROJECTS_KEY][i][SAVED_PROJECTS_PROJECT_NAME_KEY] =
        savedProjects[i].ProjectName;
      jsonObject[SAVED_PROJECTS_KEY][i][SAVED_PROJECTS_PROJECT_PATH_KEY] =
        savedProjects[i].Path.string();
      jsonObject[SAVED_PROJECTS_KEY][i][SAVED_PROJECTS_LAST_OPENED_KEY] =
        savedProjects[i].LastOpened;
      jsonObject[SAVED_PROJECTS_KEY][i][SAVED_PROJECTS_GRAPHICS_API_KEY] =
        savedProjects[i].GraphicsApi;
    }

    std::string           fileContent = jsonObject.dump(4);
    std::filesystem::path settingsPath = m_FileHandler->GetEngineSettingsPath();
    if (!m_FileHandler->DirectoyExists(settingsPath))
    {
      m_FileHandler->CreateDirectoryAt(settingsPath);
    }
    std::filesystem::path savedProjectsPath =
      settingsPath / "savedProjects.json";
    m_FileHandler->WriteToFile(savedProjectsPath.make_preferred(), fileContent);

    m_Logger->LogInfo(Log("Project list saved", "SavedProjectsIO"));
  }
}