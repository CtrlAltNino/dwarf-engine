#pragma once

#include "Launcher/ProjectList/IProjectList.h"
#include "Launcher/ProjectList/Sorter/IProjectListSorter.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class ProjectListSorter : public IProjectListSorter
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    ProjectSortOrder              m_SortOrder = ProjectSortOrder::Date;

  public:
    ProjectListSorter(std::shared_ptr<IDwarfLogger> logger);

    ~ProjectListSorter() override;

    void
    UpdateSortOrder(ProjectListColumn columnId) override;

    void
    SortProjectList(std::vector<ProjectInformation>& projectList) override;
  };
}