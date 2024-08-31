#include "ProjectList.h"
#include "IO/IProjectListIO.h"
#include "Utilities/FileHandler.h"

namespace Dwarf
{
  ProjectList::ProjectList(std::shared_ptr<IProjectListIO> projectListIO)
    : m_ProjectListIO(projectListIO)
  {
    m_ProjectList = m_ProjectListIO->LoadProjectList();
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