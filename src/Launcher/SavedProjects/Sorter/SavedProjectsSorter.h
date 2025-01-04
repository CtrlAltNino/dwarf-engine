#pragma once

#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class SavedProjectsSorter : public ISavedProjectsSorter
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    ProjectSortOrder              m_SortOrder = ProjectSortOrder::Date;

  public:
    SavedProjectsSorter(std::shared_ptr<IDwarfLogger> logger);

    ~SavedProjectsSorter() override;

    void
    UpdateSortOrder(ProjectListColumn columnId) override;

    void
    SortSavedProjects(std::vector<SavedProject>& savedProjects) override;
  };
}