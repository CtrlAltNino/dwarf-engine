#pragma once

#include "Launcher/IProjectLauncher.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
namespace Dwarf
{
#define NAME_COLUMN_INDEX (0)
#define PATH_COLUMN_INDEX (1)
#define DATE_COLUMN_INDEX (2)
#define API_COLUMN_INDEX (3)

  /// @brief Enum defining different sort orders for projects.
  enum class ProjectSortOrder
  {
    Name,
    NameReverse,
    Date,
    DateReverse,
    Api,
    ApiReverse
  };

  class ISavedProjectsSorter
  {
  public:
    virtual ~ISavedProjectsSorter() = default;

    virtual void
    UpdateSortOrder(ProjectListColumn columnId) = 0;

    virtual void
    SortSavedProjects(std::vector<SavedProject>& savedProjects) = 0;
  };
}