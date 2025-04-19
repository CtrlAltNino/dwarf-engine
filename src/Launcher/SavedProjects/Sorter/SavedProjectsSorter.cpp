#include "pch.hpp"

#include "Launcher/SavedProjects/ISavedProjects.hpp"
#include "SavedProjectsSorter.hpp"

namespace Dwarf
{
  SavedProjectsSorter::SavedProjectsSorter(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
  {
    mLogger->LogDebug(
      Log("SavedProjectsSorter created", "SavedProjectsSorter"));
  }

  SavedProjectsSorter::~SavedProjectsSorter()
  {
    mLogger->LogDebug(
      Log("SavedProjectsSorter destroyed", "SavedProjectsSorter"));
  }

  /// @brief Converts a string to lower case.
  /// @param s The source string.
  /// @return The converted output.
  auto
  ToLowerCase(std::string string) -> std::string
  {
    std::ranges::transform(string,
                           string.begin(),
                           [](auto character)
                           { return std::tolower(character, std::locale()); });
    return string;
  }

  /// @brief Name comparator for regular name sort order.
  /// @param p1 Left project.
  /// @param p2 Right project.
  /// @return Comparison result.
  auto
  ProjectNameComparator(const SavedProject& p1, const SavedProject& p2) -> bool
  {
    return ToLowerCase(p1.ProjectName) < ToLowerCase(p2.ProjectName);
  }

  /// @brief Name comparator for reverse name sort order.
  /// @param p1 Left project.
  /// @param p2 Right project.
  /// @return Comparison result.
  auto
  ProjectNameReverseComparator(const SavedProject& p1,
                               const SavedProject& p2) -> bool
  {
    return ToLowerCase(p1.ProjectName) > ToLowerCase(p2.ProjectName);
  }

  /// @brief Date comparator for regular date sort order.
  /// @param p1 Left project.
  /// @param p2 Right project.
  /// @return Comparison result.
  auto
  ProjectDateComparator(const SavedProject& p1, const SavedProject& p2) -> bool
  {
    // Accomodating for never opened projects having "-1" as a value
    int a = p1.LastOpened == -1 ? INT_MAX : p1.LastOpened;
    int b = p2.LastOpened == -1 ? INT_MAX : p2.LastOpened;

    // Sorting by project name as fallback
    return a == b ? ProjectNameComparator(p1, p2) : (a > b);
  }

  /// @brief Date comparator for reverse date sort order.
  /// @param p1 Left project.
  /// @param p2 Right project.
  /// @return Comparison result.
  auto
  ProjectDateReverseComparator(const SavedProject& p1,
                               const SavedProject& p2) -> bool
  {
    // Accomodating for never opened projects having "-1" as a value
    int a = p1.LastOpened == -1 ? INT_MAX : p1.LastOpened;
    int b = p2.LastOpened == -1 ? INT_MAX : p2.LastOpened;

    // Sorting by project name as fallback
    return a == b ? ProjectNameComparator(p1, p2) : (a < b);
  }

  /// @brief API comparator for regular API sort order.
  /// @param p1 Left project.
  /// @param p2 Right project.
  /// @return Comparison result.
  auto
  ProjectApiComparator(const SavedProject& p1, const SavedProject& p2) -> bool
  {
    return p1.GraphicsApi > p2.GraphicsApi;
  }

  /// @brief API comparator for reverse API sort order.
  /// @param p1
  /// @param p2
  /// @return
  auto
  ProjectApiReverseComparator(const SavedProject& p1,
                              const SavedProject& p2) -> bool
  {
    return p1.GraphicsApi < p2.GraphicsApi;
  }

  void
  SavedProjectsSorter::UpdateSortOrder(ProjectListColumn columnId)
  {
    mLogger->LogInfo(Log("Updating sort order", "SavedProjectsSorter"));

    switch (columnId)
    {
      case NAME_COLUMN_INDEX:
        if (mSortOrder == ProjectSortOrder::Name)
        {
          mSortOrder = ProjectSortOrder::NameReverse;
        }
        else
        {
          mSortOrder = ProjectSortOrder::Name;
        }
        break;
      case PATH_COLUMN_INDEX: break;
      case DATE_COLUMN_INDEX:
        if (mSortOrder == ProjectSortOrder::Date)
        {
          mSortOrder = ProjectSortOrder::DateReverse;
        }
        else
        {
          mSortOrder = ProjectSortOrder::Date;
        }
        break;
      case API_COLUMN_INDEX:
        if (mSortOrder == ProjectSortOrder::Api)
        {
          mSortOrder = ProjectSortOrder::ApiReverse;
        }
        else
        {
          mSortOrder = ProjectSortOrder::Api;
        }
        break;
    }
  }

  void
  SavedProjectsSorter::SortSavedProjects(
    std::vector<SavedProject>& savedProjects)
  {
    mLogger->LogInfo(Log("Sorting project list", "SavedProjectsSorter"));

    switch (mSortOrder)
    {
      case ProjectSortOrder::Name:
        std::ranges::sort(savedProjects, ProjectNameComparator);
        break;
      case ProjectSortOrder::NameReverse:
        std::ranges::sort(savedProjects, ProjectNameReverseComparator);
        break;
      case ProjectSortOrder::Date:
        std::ranges::sort(savedProjects, ProjectDateComparator);
        break;
      case ProjectSortOrder::DateReverse:
        std::ranges::sort(savedProjects, ProjectDateReverseComparator);
        break;
      case ProjectSortOrder::Api:
        std::ranges::sort(savedProjects, ProjectApiComparator);
        break;
      case ProjectSortOrder::ApiReverse:
        std::ranges::sort(savedProjects, ProjectApiReverseComparator);
        break;
    }
  }
}