#include "ProjectList.h"
#include "IO/IProjectListIO.h"
#include "Utilities/FileHandler.h"
#include <nfd.h>

namespace Dwarf
{
  ProjectList::ProjectList(
    std::shared_ptr<IProjectListIO>     projectListIO,
    std::shared_ptr<IProjectListSorter> projectListSorter)
    : m_ProjectListIO(projectListIO)
    , m_ProjectListSorter(projectListSorter)
  {
    m_ProjectList = m_ProjectListIO->LoadProjectList();
    m_ProjectListSorter->SortProjectList(m_ProjectList);
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

    nfdu8char_t*          outPath = NULL;
    nfdpickfolderu8args_t args = { 0 };
    // args.defaultPath = (const nfdu8char_t*)newProjectPath.c_str();
    //  nfdu8
    nfdresult_t result = NFD_PickFolderU8_With(&outPath, &args);

    if (result == NFD_OKAY)
    {
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
          // ProjectSorter::SortProjectList(&s_ProjectList);
          m_ProjectListSorter->SortProjectList(m_ProjectList);
          //  Aktualisierte Projektliste speicher
          m_ProjectListIO->SaveProjectList(m_ProjectList);
        }
        else
        {
          // TODO: Open modal to signal that no project could be found at the
          // given path
        }
      }
      else
      {
        // TODO: Modal to notify the project is already present in the current
        // project list
      }
      free(outPath);
    }
    else if (result == NFD_CANCEL)
    {
      // puts("User pressed cancel.");
    }
    else
    {
      // printf("Error: %s\n", NFD_GetError());
    }
  }

  void
  ProjectList::RemoveProject(const std::filesystem::path& path)
  {
    for (auto it = m_ProjectList.begin(); it != m_ProjectList.end(); ++it)
    {
      if (it->path == path)
      {
        m_ProjectList.erase(it);
        break;
      }
    }
  }

  void
  ProjectList::ChangeGraphicsApi(const std::filesystem::path& path,
                                 GraphicsApi                  api)
  {
    for (auto& project : m_ProjectList)
    {
      if (project.path == path)
      {
        project.graphicsApi = api;
        break;
      }
    }
  }

  void
  ProjectList::RegisterProjectOpening(const std::filesystem::path& path)
  {
    for (auto& project : m_ProjectList)
    {
      if (project.path == path)
      {
        project.lastOpened = time(nullptr);
        break;
      }
    }
  }

  std::vector<ProjectInformation>&
  ProjectList::GetProjectList()
  {
    return m_ProjectList;
  }
}