#pragma once

#include "../../utilities/dpch.h"

#include "ProjectLauncherUtilities.h"

#define NAME_COLUMN_INDEX (0)
#define PATH_COLUMN_INDEX (1)
#define DATE_COLUMN_INDEX (2)
#define API_COLUMN_INDEX (3)

/// @brief Enum defining different sort orders for projects.
enum class ProjectSortOrder {Name, NameReverse, Date, DateReverse, Api, ApiReverse};

/// @brief Class providing functionality to sort projects.
class ProjectSorter{
    private:
        /// @brief Currently selected sort order.
        static inline ProjectSortOrder sortOrder = ProjectSortOrder::Date;

        /// @brief Converts a string to lower case.
        /// @param s The source string.
        /// @return The converted output.
        static std::string ToLowerCase(std::string s) {
            transform(s.begin(), s.end(), s.begin(),
            [](auto c) { return std::tolower(c, std::locale()); }
            );
            return s;
        }

        /// @brief Name comparator for regular name sort order.
        /// @param p1 Left project.
        /// @param p2 Right project.
        /// @return Comparison result.
        static bool projectNameComparator(ProjectInformation p1, ProjectInformation p2) {
            return ToLowerCase(p1.name) < ToLowerCase(p2.name);
        }

        /// @brief Name comparator for reverse name sort order.
        /// @param p1 Left project.
        /// @param p2 Right project.
        /// @return Comparison result.
        static bool projectNameReverseComparator(ProjectInformation p1, ProjectInformation p2) {
            return ToLowerCase(p1.name) > ToLowerCase(p2.name);
        }

        /// @brief Date comparator for regular date sort order.
        /// @param p1 Left project.
        /// @param p2 Right project.
        /// @return Comparison result.
        static bool projectDateComparator(ProjectInformation p1, ProjectInformation p2) {
            // Accomodating for never opened projects having "-1" as a value
            int a = p1.lastOpened == -1 ? INT_MAX : p1.lastOpened;
            int b = p2.lastOpened == -1 ? INT_MAX : p2.lastOpened;
            
            // Sorting by project name as fallback
            return a == b ? projectNameComparator(p1, p2) : (a > b);
        }

        /// @brief Date comparator for reverse date sort order.
        /// @param p1 Left project.
        /// @param p2 Right project.
        /// @return Comparison result.
        static bool projectDateReverseComparator(ProjectInformation p1, ProjectInformation p2) {
            // Accomodating for never opened projects having "-1" as a value
            int a = p1.lastOpened == -1 ? INT_MAX : p1.lastOpened;
            int b = p2.lastOpened == -1 ? INT_MAX : p2.lastOpened;
            
            // Sorting by project name as fallback
            return a == b ? projectNameComparator(p1, p2) : (a < b);
        }

        /// @brief API comparator for regular API sort order.
        /// @param p1 Left project.
        /// @param p2 Right project.
        /// @return Comparison result.
        static bool projectApiComparator(ProjectInformation p1, ProjectInformation p2) {
            return p1.graphicsApi > p2.graphicsApi;
        }

        /// @brief API comparator for reverse API sort order.
        /// @param p1 
        /// @param p2 
        /// @return 
        static bool projectApiReverseComparator(ProjectInformation p1, ProjectInformation p2) {
            return p1.graphicsApi < p2.graphicsApi;
        }
    
    public:
        /// @brief Updates the sort order, depending on which column title has been clicked.
        /// @param columnId 
        static void UpdateSortOrder(int columnId) {
            switch (columnId) {
            case NAME_COLUMN_INDEX:
                if (sortOrder == ProjectSortOrder::Name) {
                    sortOrder = ProjectSortOrder::NameReverse;
                }
                else {
                    sortOrder = ProjectSortOrder::Name;
                }
                //SortProjectList();
                break;
            case PATH_COLUMN_INDEX: break;
            case DATE_COLUMN_INDEX:
                if (sortOrder == ProjectSortOrder::Date) {
                    sortOrder = ProjectSortOrder::DateReverse;
                }
                else {
                    sortOrder = ProjectSortOrder::Date;
                }
                //SortProjectList();
                break;
            case API_COLUMN_INDEX:
                if (sortOrder == ProjectSortOrder::Api) {
                    sortOrder = ProjectSortOrder::ApiReverse;
                }
                else {
                    sortOrder = ProjectSortOrder::Api;
                }
                break;
            }
        }

        /// @brief Sorts a given project list according to the currently selected sort order.
        /// @param projectList Pointer to a project list.
        static void SortProjectList(std::vector<ProjectInformation>* projectList) {
            switch (sortOrder) {
            case ProjectSortOrder::Name:
                std::sort(projectList->begin(), projectList->end(), projectNameComparator);
                break;
            case ProjectSortOrder::NameReverse:
                std::sort(projectList->begin(), projectList->end(), projectNameReverseComparator);
                break;
            case ProjectSortOrder::Date:
                std::sort(projectList->begin(), projectList->end(), projectDateComparator);
                break;
            case ProjectSortOrder::DateReverse:
                std::sort(projectList->begin(), projectList->end(), projectDateReverseComparator);
                break;
            case ProjectSortOrder::Api:
                std::sort(projectList->begin(), projectList->end(), projectApiComparator);
                break;
            case ProjectSortOrder::ApiReverse:
                std::sort(projectList->begin(), projectList->end(), projectApiReverseComparator);
                break;
            }
        }
};