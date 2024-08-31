#pragma once

#include "IO/IProjectListIO.h"
#include "Launcher/ProjectList/IO/IProjectListIO.h"
#include "Launcher/ProjectList/IProjectList.h"

namespace Dwarf
{
  class ProjectList : public IProjectList
  {
  private:
    std::vector<ProjectInformation> m_ProjectList;
    std::shared_ptr<IProjectListIO> m_ProjectListIO;

    void
    LoadProjectList();

    void
    SaveProjectList();

  public:
    ProjectList(std::shared_ptr<IProjectListIO> projectListIO);

    void
    AddProject(const std::filesystem::path& path);

    void
    OpenAddProjectDialog();

    void
    RemoveProject(const std::filesystem::path& path);

    void
    ChangeGraphicsApi(const std::filesystem::path& path, GraphicsApi api);

    void
    RegisterProjectOpening(const std::filesystem::path& path);

    std::vector<ProjectInformation>&
    GetProjectList();
  };
}