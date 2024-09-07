#include "ProjectList.h"
#include "IO/IProjectListIO.h"
#include "Utilities/FileHandler.h"
#include <nfd.h>
#include <nfd_sdl2.h>

namespace Dwarf
{
  ProjectList::ProjectList(
    std::shared_ptr<IProjectListIO>     projectListIO,
    std::shared_ptr<IProjectListSorter> projectListSorter,
    std::shared_ptr<IWindow>            window,
    std::shared_ptr<IDwarfLogger>       logger)
    : m_ProjectListIO(projectListIO)
    , m_ProjectListSorter(projectListSorter)
    , m_Window(window)
    , m_Logger(logger)
  {
    m_Logger->LogInfo(Log("Loading ProjectList...", "ProjectList"));
    m_ProjectList = m_ProjectListIO->LoadProjectList();
    m_Logger->LogInfo(Log("ProjectList loaded.", "ProjectList"));

    m_Logger->LogInfo(Log("Sorting ProjectList...", "ProjectList"));
    m_ProjectListSorter->SortProjectList(m_ProjectList);
    m_Logger->LogInfo(Log("ProjectList sorted.", "ProjectList"));

    m_Logger->LogInfo(Log("ProjectList created.", "ProjectList"));
    NFD_Init();
  }

  ProjectList::~ProjectList()
  {
    NFD_Quit();
  }

  void
  ProjectList::AddProject(const std::filesystem::path& path)
  {
    ProjectInformation info;
    info.path = path;
    info.name = path.filename().string();
    info.lastOpened = 0;
    info.graphicsApi = GraphicsApi::OpenGL;
    m_ProjectList.push_back(info);
  }

  void
  ProjectList::OpenAddProjectDialog()
  {
    // TODO: implement file dialog in another thread to not interrupt
    // TODO: rename "open" button to "add project" or something
    // TODO: focus on the opened dialog when trying to return to the project

    m_Logger->LogInfo(Log("Opening Add Project Dialog...", "ProjectList"));

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
        Log("Selected path: " + std::string(outPath), "ProjectList"));
      // check if a project with the same path already exists
      bool alreadyPresent = false;

      for (int i = 0; (i < m_ProjectList.size()) && !alreadyPresent; i++)
      {
        alreadyPresent = m_ProjectList[i].path.t == outPath;
      }

      if (!alreadyPresent)
      {
        ProjectInformation newProject = m_ProjectListIO->LoadProject(outPath);
        if (newProject.name != "")
        {

          m_ProjectList.push_back(newProject);
          m_Logger->LogInfo(
            Log("Project added to project list.", "ProjectList"));

          m_ProjectListSorter->SortProjectList(m_ProjectList);
          m_Logger->LogInfo(Log("Project list sorted.", "ProjectList"));

          m_ProjectListIO->SaveProjectList(m_ProjectList);
          m_Logger->LogInfo(Log("Project list saved.", "ProjectList"));
        }
        else
        {
          // TODO: Open modal to signal that no project could be found at the
          // given path
          m_Logger->LogWarn(
            Log("No project found at the given path.", "ProjectList"));
        }
      }
      else
      {
        // TODO: Modal to notify the project is already present in the current
        // project list
        m_Logger->LogWarn(
          Log("Project already present in project list.", "ProjectList"));
      }
      free(outPath);
    }
    else if (result == NFD_CANCEL)
    {
      // puts("User pressed cancel.");
      m_Logger->LogInfo(Log("User pressed cancel.", "ProjectList"));
    }
    else
    {
      // printf("Error: %s\n", NFD_GetError());
      m_Logger->LogError(Log(
        fmt::format("Error: {}", std::string(NFD_GetError())), "ProjectList"));
    }
  }

  void
  ProjectList::RemoveProject(const std::filesystem::path& path)
  {
    m_Logger->LogInfo(Log("Removing project...", "ProjectList"));
    for (auto it = m_ProjectList.begin(); it != m_ProjectList.end(); ++it)
    {
      if (it->path == path)
      {
        m_ProjectList.erase(it);
        m_Logger->LogInfo(Log("Project removed.", "ProjectList"));
        break;
      }

      if (it == m_ProjectList.end())
      {
        m_Logger->LogWarn(Log("Project to remove not found.", "ProjectList"));
      }
    }
  }

  void
  ProjectList::ChangeGraphicsApi(const std::filesystem::path& path,
                                 GraphicsApi                  api)
  {
    m_Logger->LogInfo(Log("Changing graphics API...", "ProjectList"));
    for (auto& project : m_ProjectList)
    {
      if (project.path == path)
      {
        project.graphicsApi = api;
        m_Logger->LogInfo(Log("Graphics API changed.", "ProjectList"));
        m_ProjectListSorter->SortProjectList(m_ProjectList);
        m_ProjectListIO->SaveProjectList(m_ProjectList);
        m_Logger->LogInfo(Log("Project list saved.", "ProjectList"));
        break;
      }

      if (&project == &m_ProjectList.back())
      {
        m_Logger->LogWarn(Log("Project to change not found.", "ProjectList"));
      }
    }
  }

  void
  ProjectList::RegisterProjectOpening(const std::filesystem::path& path)
  {
    m_Logger->LogInfo(Log("Registering project opening...", "ProjectList"));
    for (auto& project : m_ProjectList)
    {
      m_Logger->LogInfo(Log("Checking project...", "ProjectList"));
      if (project.path.t == path)
      {
        project.lastOpened = time(nullptr);
        m_Logger->LogInfo(Log("Project opening registered.", "ProjectList"));
        m_ProjectListSorter->SortProjectList(m_ProjectList);
        m_Logger->LogInfo(Log("Project list sorted.", "ProjectList"));
        m_ProjectListIO->SaveProjectList(m_ProjectList);
        m_Logger->LogInfo(Log("Project list saved.", "ProjectList"));
        break;
      }

      if (&project == &m_ProjectList.back())
      {
        m_Logger->LogWarn(
          Log("Project to register new launch not found.", "ProjectList"));
      }
    }
  }

  std::vector<ProjectInformation>&
  ProjectList::GetProjectList()
  {
    return m_ProjectList;
  }
}