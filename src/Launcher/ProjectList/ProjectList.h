#pragma once

#include "IO/IProjectListIO.h"
#include "Launcher/ProjectList/IO/IProjectListIO.h"
#include "Launcher/ProjectList/IProjectList.h"
#include "Launcher/ProjectList/Sorter/IProjectListSorter.h"
#include "Logging/IDwarfLogger.h"
#include "Window/IWindow.h"

namespace Dwarf
{
  class ProjectList : public IProjectList
  {
  private:
    std::vector<ProjectInformation>     m_ProjectList;
    std::shared_ptr<IProjectListIO>     m_ProjectListIO;
    std::shared_ptr<IProjectListSorter> m_ProjectListSorter;
    std::shared_ptr<IWindow>            m_Window;
    std::shared_ptr<IDwarfLogger>       m_Logger;

  public:
    ProjectList(std::shared_ptr<IProjectListIO>     projectListIO,
                std::shared_ptr<IProjectListSorter> projectSorter,
                std::shared_ptr<IWindow>            window,
                std::shared_ptr<IDwarfLogger>       logger);

    ~ProjectList() override;
    void
    AddProject(const std::filesystem::path& path) override;

    void
    OpenAddProjectDialog() override;

    void
    RemoveProject(const std::filesystem::path& path) override;

    void
    ChangeGraphicsApi(const std::filesystem::path& path,
                      GraphicsApi                  api) override;

    void
    RegisterProjectOpening(const std::filesystem::path& path) override;

    std::vector<ProjectInformation>&
    GetProjectList() override;
  };
}