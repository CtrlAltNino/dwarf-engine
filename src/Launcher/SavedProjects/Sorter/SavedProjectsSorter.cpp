#include "SavedProjectsSorter.h"
#include "Launcher/SavedProjects/ISavedProjects.h"

namespace Dwarf
{
  SavedProjectsSorter::SavedProjectsSorter(std::shared_ptr<IDwarfLogger> logger)
    : m_Logger(logger)
  {
    m_Logger->LogDebug(
      Log("SavedProjectsSorter created", "SavedProjectsSorter"));
  }

  SavedProjectsSorter::~SavedProjectsSorter()
  {
    m_Logger->LogDebug(
      Log("SavedProjectsSorter destroyed", "SavedProjectsSorter"));
  }

  /// @brief Converts a string to lower case.
  /// @param s The source string.
  /// @return The converted output.
  std::string
  ToLowerCase(std::string s)
  {
    transform(s.begin(),
              s.end(),
              s.begin(),
              [](auto c) { return std::tolower(c, std::locale()); });
    return s;
  }

  /// @brief Name comparator for regular name sort order.
  /// @param p1 Left project.
  /// @param p2 Right project.
  /// @return Comparison result.
  bool
  ProjectNameComparator(SavedProject p1, SavedProject p2)
  {
    return ToLowerCase(p1.ProjectName) < ToLowerCase(p2.ProjectName);
  }

  /// @brief Name comparator for reverse name sort order.
  /// @param p1 Left project.
  /// @param p2 Right project.
  /// @return Comparison result.
  bool
  ProjectNameReverseComparator(SavedProject p1, SavedProject p2)
  {
    return ToLowerCase(p1.ProjectName) > ToLowerCase(p2.ProjectName);
  }

  /// @brief Date comparator for regular date sort order.
  /// @param p1 Left project.
  /// @param p2 Right project.
  /// @return Comparison result.
  bool
  ProjectDateComparator(SavedProject p1, SavedProject p2)
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
  bool
  ProjectDateReverseComparator(SavedProject p1, SavedProject p2)
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
  bool
  ProjectApiComparator(SavedProject p1, SavedProject p2)
  {
    return p1.GraphicsApi > p2.GraphicsApi;
  }

  /// @brief API comparator for reverse API sort order.
  /// @param p1
  /// @param p2
  /// @return
  bool
  ProjectApiReverseComparator(SavedProject p1, SavedProject p2)
  {
    return p1.GraphicsApi < p2.GraphicsApi;
  }

  void
  SavedProjectsSorter::UpdateSortOrder(ProjectListColumn columnId)
  {
    m_Logger->LogInfo(Log("Updating sort order", "SavedProjectsSorter"));

    switch (columnId)
    {
      case NAME_COLUMN_INDEX:
        if (m_SortOrder == ProjectSortOrder::Name)
        {
          m_SortOrder = ProjectSortOrder::NameReverse;
        }
        else
        {
          m_SortOrder = ProjectSortOrder::Name;
        }
        break;
      case PATH_COLUMN_INDEX: break;
      case DATE_COLUMN_INDEX:
        if (m_SortOrder == ProjectSortOrder::Date)
        {
          m_SortOrder = ProjectSortOrder::DateReverse;
        }
        else
        {
          m_SortOrder = ProjectSortOrder::Date;
        }
        break;
      case API_COLUMN_INDEX:
        if (m_SortOrder == ProjectSortOrder::Api)
        {
          m_SortOrder = ProjectSortOrder::ApiReverse;
        }
        else
        {
          m_SortOrder = ProjectSortOrder::Api;
        }
        break;
    }
  }

  void
  SavedProjectsSorter::SortSavedProjects(
    std::vector<SavedProject>& savedProjects)
  {
    m_Logger->LogInfo(Log("Sorting project list", "SavedProjectsSorter"));

    switch (m_SortOrder)
    {
      case ProjectSortOrder::Name:
        std::sort(
          savedProjects.begin(), savedProjects.end(), ProjectNameComparator);
        break;
      case ProjectSortOrder::NameReverse:
        std::sort(savedProjects.begin(),
                  savedProjects.end(),
                  ProjectNameReverseComparator);
        break;
      case ProjectSortOrder::Date:
        std::sort(
          savedProjects.begin(), savedProjects.end(), ProjectDateComparator);
        break;
      case ProjectSortOrder::DateReverse:
        std::sort(savedProjects.begin(),
                  savedProjects.end(),
                  ProjectDateReverseComparator);
        break;
      case ProjectSortOrder::Api:
        std::sort(
          savedProjects.begin(), savedProjects.end(), ProjectApiComparator);
        break;
      case ProjectSortOrder::ApiReverse:
        std::sort(savedProjects.begin(),
                  savedProjects.end(),
                  ProjectApiReverseComparator);
        break;
    }
  }
}