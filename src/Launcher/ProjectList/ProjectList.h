#pragma once

#include "IO/IProjectListIO.h"
#include "Launcher/ProjectList/IO/IProjectListIO.h"
#include "Launcher/ProjectList/IProjectList.h"
#include "Launcher/ProjectList/Sorter/IProjectListSorter.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"
#include <memory>
// #include "Window/IWindow.h"

namespace Dwarf
{
  class ProjectList : public IProjectList
  {
  private:
    std::vector<ProjectInformation> m_ProjectList;
    IDwarfLogger&                   m_Logger;
    IProjectListIO&                 m_ProjectListIO;
    IProjectListSorter&             m_ProjectListSorter;

  public:
    ProjectList(IDwarfLogger&       logger,
                IProjectListIO&     projectListIO,
                IProjectListSorter& projectListSorter);

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