#pragma once

#include "Launcher/SavedProjects/ISavedProjects.h"

namespace Dwarf
{
#define NAME_COLUMN_INDEX (0)
#define PATH_COLUMN_INDEX (1)
#define DATE_COLUMN_INDEX (2)
#define API_COLUMN_INDEX (3)

  /// @brief Enum defining different sort orders for projects.
  enum class ProjectSortOrder : uint8_t
  {
    Name,
    NameReverse,
    Date,
    DateReverse,
    Api,
    ApiReverse
  };

  /**
   * @brief Class that sorts lists of saved project data
   *
   */
  class ISavedProjectsSorter
  {
  public:
    virtual ~ISavedProjectsSorter() = default;

    /**
     * @brief Changes the desired sort order
     *
     * @param columnId Selectes the column to sort for
     */
    virtual void
    UpdateSortOrder(ProjectListColumn columnId) = 0;

    /**
     * @brief Sorts a provided list
     *
     * @param savedProjects List of saved projects
     */
    virtual void
    SortSavedProjects(std::vector<SavedProject>& savedProjects) = 0;
  };
}