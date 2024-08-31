#pragma once

#include "Launcher/ProjectList/IProjectList.h"
#include "Launcher/ProjectList/Sorter/IProjectListSorter.h"

namespace Dwarf
{
  class ProjectListSorter : public IProjectListSorter
  {
  private:
    ProjectSortOrder m_SortOrder = ProjectSortOrder::Date;

  public:
    ProjectListSorter();

    void
    UpdateSortOrder(ProjectListColumn columnId) override;

    void
    SortProjectList(std::vector<ProjectInformation>& projectList) override;
  };
}