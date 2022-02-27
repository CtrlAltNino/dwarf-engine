#pragma once

#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

#include "ProjectLauncherUtilities.h"

enum class ProjectSortOrder {Name, NameReverse, Date, DateReverse, Api, ApiReverse};

class ProjectSorter{
    private:
        static inline ProjectSortOrder sortOrder;

        static bool projectNameComparator(ProjectInformation p1, ProjectInformation p2) {
            return p1.name > p2.name;
        }

        static bool projectNameReverseComparator(ProjectInformation p1, ProjectInformation p2) {
            return p1.name < p2.name;
        }

        static bool projectDateComparator(ProjectInformation p1, ProjectInformation p2) {
            return p1.lastOpened > p2.lastOpened;
        }

        static bool projectDateReverseComparator(ProjectInformation p1, ProjectInformation p2) {
            return p1.lastOpened < p2.lastOpened;
        }

        static bool projectApiComparator(ProjectInformation p1, ProjectInformation p2) {
            return p1.graphicsApi > p2.graphicsApi;
        }

        static bool projectApiReverseComparator(ProjectInformation p1, ProjectInformation p2) {
            return p1.graphicsApi < p2.graphicsApi;
        }
    
    public:
        static void UpdateSortOrder(int columnId) {
            switch (columnId) {
            case 0:
                if (sortOrder == ProjectSortOrder::Name) {
                    sortOrder = ProjectSortOrder::NameReverse;
                }
                else {
                    sortOrder = ProjectSortOrder::Name;
                }
                //SortProjectList();
                break;
            case 1: break;
            case 2:
                if (sortOrder == ProjectSortOrder::Date) {
                    sortOrder = ProjectSortOrder::DateReverse;
                }
                else {
                    sortOrder = ProjectSortOrder::Date;
                }
                //SortProjectList();
                break;
            case 3:
                if (sortOrder == ProjectSortOrder::Api) {
                    sortOrder = ProjectSortOrder::ApiReverse;
                }
                else {
                    sortOrder = ProjectSortOrder::Api;
                }
                //SortProjectList();
                break;
            }
        }

        static std::vector<ProjectInformation> SortProjectList(std::vector<ProjectInformation> projectList) {
            switch (sortOrder) {
            case ProjectSortOrder::Name:
                std::sort(projectList.begin(), projectList.end(), projectNameComparator);
                break;
            case ProjectSortOrder::NameReverse:
                std::sort(projectList.begin(), projectList.end(), projectNameReverseComparator);
                break;
            case ProjectSortOrder::Date:
                std::sort(projectList.begin(), projectList.end(), projectDateComparator);
                break;
            case ProjectSortOrder::DateReverse:
                std::sort(projectList.begin(), projectList.end(), projectDateReverseComparator);
                break;
            case ProjectSortOrder::Api:
                std::sort(projectList.begin(), projectList.end(), projectApiComparator);
                break;
            case ProjectSortOrder::ApiReverse:
                std::sort(projectList.begin(), projectList.end(), projectApiReverseComparator);
                break;
            }
            return projectList;
        }
};