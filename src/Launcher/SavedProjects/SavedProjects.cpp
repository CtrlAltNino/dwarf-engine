#include "SavedProjects.h"
#include "IO/ISavedProjectsIO.h"
#include "ISavedProjects.h"
#include "Project/IProjectSettings.h"
#include <nfd.h>
#include <nfd_sdl2.h>

namespace Dwarf
{
  SavedProjects::SavedProjects(
    std::shared_ptr<IDwarfLogger>         logger,
    std::shared_ptr<ISavedProjectsIO>     savedProjectsIO,
    std::shared_ptr<ISavedProjectsSorter> savedProjectsSorter,
    std::shared_ptr<IProjectSettingsIO>   projectSettingsIO)
    : m_Logger(logger)
    , m_SavedProjectsIO(savedProjectsIO)
    , m_SavedProjectsSorter(savedProjectsSorter)
    , m_ProjectSettingsIO(projectSettingsIO)
  {
    m_Logger->LogInfo(Log("Loading SavedProjects...", "SavedProjects"));
    m_SavedProjects = m_SavedProjectsIO->LoadSavedProjects();
    m_Logger->LogInfo(Log("SavedProjects loaded.", "SavedProjects"));

    m_Logger->LogDebug(Log("Sorting SavedProjects...", "SavedProjects"));
    m_SavedProjectsSorter->SortSavedProjects(m_SavedProjects);
    m_Logger->LogDebug(Log("SavedProjects sorted.", "SavedProjects"));

    m_Logger->LogDebug(Log("SavedProjects created.", "SavedProjects"));
    NFD_Init();
    m_Logger->LogDebug(Log("Native File Dialog initialized.", "SavedProjects"));
  }

  SavedProjects::~SavedProjects()
  {
    m_Logger->LogInfo(Log("Destroying SavedProjects...", "SavedProjects"));
    NFD_Quit();
    m_Logger->LogInfo(Log("Native File Dialog destroyed.", "SavedProjects"));
  }

  void
  SavedProjects::AddProject(const std::filesystem::path& path)
  {
    m_Logger->LogDebug(Log("Adding project...", "SavedProjects"));
    SavedProject info;
    info.Path = path;
    info.ProjectName = path.filename().string();
    info.LastOpened = 0;
    info.GraphicsApi = GraphicsApi::OpenGL;
    m_SavedProjects.push_back(info);
    m_SavedProjectsIO->SaveSavedProjects(m_SavedProjects);
    m_Logger->LogDebug(Log("Project added.", "SavedProjects"));
  }

  void
  SavedProjects::OpenAddProjectDialog()
  {
    // TODO: implement file dialog in another thread to not interrupt
    // TODO: rename "open" button to "add project" or something
    // TODO: focus on the opened dialog when trying to return to the project

    m_Logger->LogInfo(Log("Opening Add Project Dialog...", "SavedProjects"));

    nfdu8char_t*          outPath = NULL;
    nfdpickfolderu8args_t args = { 0 };
    // NFD_GetNativeWindowFromSDLWindow(
    //   m_Window.lock()->GetNativeWindow() /* SDL_Window* */,
    //   &args.parentWindow);
    // args.defaultPath = (const nfdu8char_t*)newProjectPath.c_str();
    //  nfdu8
    nfdresult_t result = NFD_PickFolderU8_With(&outPath, &args);

    if (result == NFD_OKAY)
    {
      m_Logger->LogInfo(
        Log("Selected path: " + std::string(outPath), "SavedProjects"));
      // check if a project with the same path already exists
      bool alreadyPresent = false;

      for (int i = 0; (i < m_SavedProjects.size()) && !alreadyPresent; i++)
      {
        alreadyPresent = m_SavedProjects[i].Path == outPath;
      }

      if (!alreadyPresent)
      {
        std::optional<ProjectSettingsData> newProject =
          m_ProjectSettingsIO->LoadProjectSettings(outPath);
        if (newProject)
        {
          SavedProject newSavedProject;
          newSavedProject.ProjectName = newProject->ProjectName;
          newSavedProject.GraphicsApi = newProject->GraphicsApi;
          newSavedProject.LastOpened = 0;
          newSavedProject.Path = outPath;

          m_SavedProjects.push_back(newSavedProject);
          m_Logger->LogInfo(
            Log("Project added to project list.", "SavedProjects"));

          m_SavedProjectsSorter->SortSavedProjects(m_SavedProjects);
          m_Logger->LogDebug(Log("Project list sorted.", "SavedProjects"));

          m_SavedProjectsIO->SaveSavedProjects(m_SavedProjects);
          m_Logger->LogDebug(Log("Project list saved.", "SavedProjects"));
        }
        else
        {
          // TODO: Open modal to signal that no project could be found at the
          // given path
          m_Logger->LogWarn(
            Log("No project found at the given path.", "SavedProjects"));
        }
      }
      else
      {
        // TODO: Modal to notify the project is already present in the current
        // project list
        m_Logger->LogWarn(
          Log("Project already present in project list.", "SavedProjects"));
      }
      free(outPath);
    }
    else if (result == NFD_CANCEL)
    {
      m_Logger->LogInfo(Log("User pressed cancel.", "SavedProjects"));
    }
    else
    {
      m_Logger->LogError(
        Log(fmt::format("Error: {}", std::string(NFD_GetError())),
            "SavedProjects"));
    }
  }

  void
  SavedProjects::RemoveProject(const std::filesystem::path& path)
  {
    m_Logger->LogInfo(Log("Removing project...", "SavedProjects"));
    for (auto it = m_SavedProjects.begin(); it != m_SavedProjects.end(); ++it)
    {
      if (it->Path == path)
      {
        m_Logger->LogInfo(Log("Project removed.", "SavedProjects"));
        m_SavedProjects.erase(it);
        m_SavedProjectsIO->SaveSavedProjects(m_SavedProjects);
        return;
      }
    }

    m_Logger->LogWarn(Log("Project to remove not found.", "SavedProjects"));
  }

  void
  SavedProjects::ChangeGraphicsApi(const std::filesystem::path& path,
                                   GraphicsApi                  api)
  {
    m_Logger->LogDebug(Log("Changing graphics API...", "SavedProjects"));
    for (auto& project : m_SavedProjects)
    {
      if (project.Path == path)
      {
        project.GraphicsApi = api;
        m_Logger->LogInfo(Log("Graphics API changed.", "SavedProjects"));
        m_SavedProjectsSorter->SortSavedProjects(m_SavedProjects);
        m_SavedProjectsIO->SaveSavedProjects(m_SavedProjects);
        m_Logger->LogDebug(Log("Project list saved.", "SavedProjects"));
        break;
      }

      if (&project == &m_SavedProjects.back())
      {
        m_Logger->LogWarn(Log("Project to change not found.", "SavedProjects"));
      }
    }
  }

  void
  SavedProjects::RegisterProjectOpening(const std::filesystem::path& path)
  {
    m_Logger->LogInfo(Log("Registering project opening...", "SavedProjects"));
    for (auto& project : m_SavedProjects)
    {
      m_Logger->LogInfo(Log("Checking project...", "SavedProjects"));
      if (project.Path == path)
      {
        project.LastOpened = time(nullptr);
        m_Logger->LogInfo(Log("Project opening registered.", "SavedProjects"));
        m_SavedProjectsSorter->SortSavedProjects(m_SavedProjects);
        m_Logger->LogInfo(Log("Project list sorted.", "SavedProjects"));
        m_SavedProjectsIO->SaveSavedProjects(m_SavedProjects);
        m_Logger->LogInfo(Log("Project list saved.", "SavedProjects"));
        break;
      }

      if (&project == &m_SavedProjects.back())
      {
        m_Logger->LogWarn(
          Log("Project to register new launch not found.", "SavedProjects"));
      }
    }
  }

  std::vector<SavedProject>&
  SavedProjects::GetSavedProjects()
  {
    return m_SavedProjects;
  }
}