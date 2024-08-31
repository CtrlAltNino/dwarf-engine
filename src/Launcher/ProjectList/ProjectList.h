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

    virtual void
    AddProject(const std::filesystem::path& path) = 0;

    virtual void
    OpenAddProjectDialog() = 0;

    virtual void
    RemoveProject(const std::filesystem::path& path) = 0;

    virtual void
    ChangeGraphicsApi(const std::filesystem::path& path, GraphicsApi api) = 0;

    virtual void
    RegisterProjectOpening(const std::filesystem::path& path) = 0;

    virtual std::vector<ProjectInformation>&
    GetProjectList() = 0;
  };
}