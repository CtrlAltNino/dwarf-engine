#pragma once

#include "Launcher/SavedProjects/ISavedProjects.hpp"
#include "Launcher/SavedProjects/Sorter/ISavedProjectsSorter.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class SavedProjectsSorter : public ISavedProjectsSorter
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    ProjectSortOrder              mSortOrder = ProjectSortOrder::Date;

  public:
    SavedProjectsSorter(std::shared_ptr<IDwarfLogger> logger);

    ~SavedProjectsSorter() override;

    /**
     * @brief Changes the desired sort order
     *
     * @param columnId Selectes the column to sort for
     */
    void
    UpdateSortOrder(ProjectListColumn columnId) override;

    /**
     * @brief Sorts a provided list
     *
     * @param savedProjects List of saved projects
     */
    void
    SortSavedProjects(std::vector<SavedProject>& savedProjects) override;
  };
}