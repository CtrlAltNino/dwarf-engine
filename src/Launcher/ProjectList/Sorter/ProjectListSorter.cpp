#include "ProjectListSorter.h"

namespace Dwarf
{
  ProjectListSorter::ProjectListSorter(std::shared_ptr<IDwarfLogger> logger)
    : m_Logger(logger)
  {
    m_Logger->LogInfo(Log("ProjectListSorter created", "ProjectListSorter"));
  }

  ProjectListSorter::~ProjectListSorter()
  {
    m_Logger->LogInfo(Log("ProjectListSorter destroyed", "ProjectListSorter"));
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
  ProjectNameComparator(ProjectInformation p1, ProjectInformation p2)
  {
    return ToLowerCase(p1.name) < ToLowerCase(p2.name);
  }

  /// @brief Name comparator for reverse name sort order.
  /// @param p1 Left project.
  /// @param p2 Right project.
  /// @return Comparison result.
  bool
  ProjectNameReverseComparator(ProjectInformation p1, ProjectInformation p2)
  {
    return ToLowerCase(p1.name) > ToLowerCase(p2.name);
  }

  /// @brief Date comparator for regular date sort order.
  /// @param p1 Left project.
  /// @param p2 Right project.
  /// @return Comparison result.
  bool
  ProjectDateComparator(ProjectInformation p1, ProjectInformation p2)
  {
    // Accomodating for never opened projects having "-1" as a value
    int a = p1.lastOpened == -1 ? INT_MAX : p1.lastOpened;
    int b = p2.lastOpened == -1 ? INT_MAX : p2.lastOpened;

    // Sorting by project name as fallback
    return a == b ? ProjectNameComparator(p1, p2) : (a > b);
  }

  /// @brief Date comparator for reverse date sort order.
  /// @param p1 Left project.
  /// @param p2 Right project.
  /// @return Comparison result.
  bool
  ProjectDateReverseComparator(ProjectInformation p1, ProjectInformation p2)
  {
    // Accomodating for never opened projects having "-1" as a value
    int a = p1.lastOpened == -1 ? INT_MAX : p1.lastOpened;
    int b = p2.lastOpened == -1 ? INT_MAX : p2.lastOpened;

    // Sorting by project name as fallback
    return a == b ? ProjectNameComparator(p1, p2) : (a < b);
  }

  /// @brief API comparator for regular API sort order.
  /// @param p1 Left project.
  /// @param p2 Right project.
  /// @return Comparison result.
  bool
  ProjectApiComparator(ProjectInformation p1, ProjectInformation p2)
  {
    return p1.graphicsApi > p2.graphicsApi;
  }

  /// @brief API comparator for reverse API sort order.
  /// @param p1
  /// @param p2
  /// @return
  bool
  ProjectApiReverseComparator(ProjectInformation p1, ProjectInformation p2)
  {
    return p1.graphicsApi < p2.graphicsApi;
  }

  void
  ProjectListSorter::UpdateSortOrder(ProjectListColumn columnId)
  {
    m_Logger->LogInfo(Log("Updating sort order", "ProjectListSorter"));

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
        // SortProjectList();
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
        // SortProjectList();
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
  ProjectListSorter::SortProjectList(
    std::vector<ProjectInformation>& projectList)
  {
    m_Logger->LogInfo(Log("Sorting project list", "ProjectListSorter"));

    switch (m_SortOrder)
    {
      case ProjectSortOrder::Name:
        std::sort(
          projectList.begin(), projectList.end(), ProjectNameComparator);
        break;
      case ProjectSortOrder::NameReverse:
        std::sort(
          projectList.begin(), projectList.end(), ProjectNameReverseComparator);
        break;
      case ProjectSortOrder::Date:
        std::sort(
          projectList.begin(), projectList.end(), ProjectDateComparator);
        break;
      case ProjectSortOrder::DateReverse:
        std::sort(
          projectList.begin(), projectList.end(), ProjectDateReverseComparator);
        break;
      case ProjectSortOrder::Api:
        std::sort(projectList.begin(), projectList.end(), ProjectApiComparator);
        break;
      case ProjectSortOrder::ApiReverse:
        std::sort(
          projectList.begin(), projectList.end(), ProjectApiReverseComparator);
        break;
    }
  }
}