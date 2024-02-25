#pragma once

#include <nlohmann/json.hpp>
#include <nfd.h>

#include "Utilities/FileHandler.h"
#include "Launcher/ProjectLauncherUtilities.h"
#include "Launcher/ProjectSorter.h"

namespace Dwarf
{

    class ProjectListHandler
    {
    private:
        /// @brief Currently loaded list of informations regarding projects.
        static inline std::vector<ProjectInformation> s_ProjectList;

        /// @brief Saves the currently loaded list of project informations back to the disk.
        /// @return True if successful, false if not.
        static bool SaveProjectList()
        {
            bool sucess = true;
            nlohmann::json jsonObject;
            for (int i = 0; i < s_ProjectList.size(); i++)
            {
                jsonObject["projects"][i]["name"] = s_ProjectList[i].name;
                jsonObject["projects"][i]["path"] = s_ProjectList[i].path.string();
                jsonObject["projects"][i]["lastOpened"] = s_ProjectList[i].lastOpened;
                jsonObject["projects"][i]["graphicsApi"] = s_ProjectList[i].graphicsApi;
            }

            std::string fileContent = jsonObject.dump(4);
            std::filesystem::path settingsPath = FileHandler::GetProjectSettingsPath();
            if (!FileHandler::CheckIfDirectoyExists(settingsPath))
            {
                FileHandler::CreateDirectory(settingsPath);
            }
            std::filesystem::path savedProjectsPath = settingsPath / "savedProjects.json";
            FileHandler::WriteToFile(savedProjectsPath.make_preferred(), fileContent);
            return sucess;
        }

    public:
        /// @brief Returns the list of project informations.
        /// @return Pointer to the list of project informations.
        static std::vector<ProjectInformation> *GetProjectList()
        {
            return &s_ProjectList;
        }

        /// @brief Returns the project information of a given project.
        /// @param id ID of the project.
        /// @return Project information of the given project.
        static ProjectInformation GetProjectInformation(int id)
        {
            ProjectInformation projectInformation;
            if (id < s_ProjectList.size())
            {
                projectInformation = s_ProjectList[id];
            }
            return projectInformation;
        }

        /// @brief Reads the project information from a given project path.
        /// @param path Path to the project.
        /// @return Project information of a project loaded from the disk.
        static ProjectInformation ExtractProjectInformation(std::filesystem::path path)
        {
            std::filesystem::path projectSettingsPath = path / "projectSettings.dproj";
            std::string fileContent = FileHandler::ReadFile(projectSettingsPath);

            ProjectInformation foundInfo;

            if (!fileContent.empty())
            {
                nlohmann::json jsonObject = nlohmann::json::parse(fileContent);

                if (jsonObject.contains("projectInformation"))
                {
                    foundInfo.name = jsonObject["projectInformation"]["projectName"];
                    foundInfo.path = path;
                    foundInfo.graphicsApi = jsonObject["projectInformation"]["graphicsApi"];
                    foundInfo.lastOpened = jsonObject.contains("lastOpened") ? (int)jsonObject["projectInformation"]["lastOpened"] : -1;
                }
            }

            return foundInfo;
        }

        /// @brief Is loading the list of saved projects on the users computer into the global variable.
        /// @return True if successful, false if not.
        static bool LoadProjectList()
        {
            s_ProjectList.clear();
            bool success = true;
            std::filesystem::path savedProjectsPath = FileHandler::GetProjectSettingsPath() / "savedProjects.json";
            std::string fileContent = FileHandler::ReadFile(savedProjectsPath);
            if (!fileContent.empty())
            {
                nlohmann::json jsonObject = nlohmann::json::parse(fileContent);

                if (jsonObject.contains("projects"))
                {
                    for (int i = 0; i < jsonObject["projects"].size(); i++)
                    {
                        ProjectInformation projectToAdd;
                        if (jsonObject["projects"][i].contains("name"))
                        {
                            projectToAdd.name = jsonObject["projects"][i]["name"];
                        }

                        if (jsonObject["projects"][i].contains("path"))
                        {
                            projectToAdd.path = std::filesystem::path(jsonObject["projects"][i]["path"].get<std::filesystem::path>());
                        }

                        if (jsonObject["projects"][i].contains("lastOpened"))
                        {
                            projectToAdd.lastOpened = jsonObject["projects"][i]["lastOpened"];
                        }

                        if (jsonObject["projects"][i].contains("graphicsApi"))
                        {
                            projectToAdd.graphicsApi = jsonObject["projects"][i]["graphicsApi"];
                        }

                        s_ProjectList.push_back({projectToAdd});
                    }

                    // After adding all the projects, sort them
                    ProjectSorter::SortProjectList(&s_ProjectList);
                }
            }
            return success;
        }

        /// @brief Adds a project to the project list and saves the new list to the disk.
        /// @param projectInformation The information of the project to add.
        /// @return True if project is already present in list, false if not.
        static bool AddProject(ProjectInformation projectInformation)
        {
            bool alreadyPresent = false;
            for (int i = 0; (i < s_ProjectList.size()) && !alreadyPresent; i++)
            {
                alreadyPresent = s_ProjectList[i].path == projectInformation.path;
            }

            if (!alreadyPresent)
            {
                s_ProjectList.push_back(projectInformation);
                ProjectSorter::SortProjectList(&s_ProjectList);
                // Aktualisierte Projektliste speichern
                SaveProjectList();
            }

            return alreadyPresent;
        }

        /// @brief Adds a project to the project list and saves the new list to the disk.
        /// @param projectPath Path to the project to add.
        /// @return True if project is already present in list, false if not.
        static bool AddProjectByPath(std::filesystem::path projectPath)
        {
            return AddProject(ExtractProjectInformation(projectPath));
        }

        /// @brief Opens a native file dialog to add a project to the project list.
        static void OpenAddProjectWindow()
        {
            // File Dialog um einen Pfad zu kriegen
            // Im Pfad nach einer projectSettings.dproj suchen
            // Projectinformation (Name, Pfad, letzte Modifikationszeit)
            nfdchar_t *outPath = NULL;
            // const nfdchar_t *filter = "dproj";

            // TODO: implement file dialog in another thread to not interrupt
            // TODO: rename "open" button to "add project" or something
            // TODO: focus on the opened dialog when trying to return to the project launcher window
            //(if that is not the default behaviour, finding out when implementing the dialog in another thread)
            // nfdresult_t result = NFD_OpenDialog(filter, NULL, &outPath);
            nfdresult_t result = NFD_PickFolder(NULL, &outPath);

            if (result == NFD_OKAY)
            {
                // check if a project with the same path already exists
                bool alreadyPresent = false;
                // outPath = PathUtilities::ConvertBackSlashesToForwardSlashes(outPath);

                for (int i = 0; (i < s_ProjectList.size()) && !alreadyPresent; i++)
                {
                    alreadyPresent = s_ProjectList[i].path == outPath;
                }

                if (!alreadyPresent)
                {
                    ProjectInformation newProject = ExtractProjectInformation(outPath);
                    if (newProject.name != "")
                    {

                        s_ProjectList.push_back(newProject);
                        ProjectSorter::SortProjectList(&s_ProjectList);
                        // Aktualisierte Projektliste speichern
                        SaveProjectList();
                    }
                    else
                    {
                        // TODO: Open modal to signal that no project could be found at the given path
                    }
                }
                else
                {
                    // TODO: Modal to notify the project is already present in the current project list
                }
                free(outPath);
            }
            else if (result == NFD_CANCEL)
            {
                // puts("User pressed cancel.");
            }
            else
            {
                // printf("Error: %s\n", NFD_GetError());
            }
        }

        /// @brief Changes the graphics API of a project.
        /// @param id Project ID.
        /// @param api Graphics API.
        static void ChangeGraphicsApi(int id, GraphicsApi api)
        {
            // ProjectInformation info = ExtractProjectInformation(projectList[id].path.c_str());
            std::filesystem::path projectSettingsPath = (s_ProjectList[id].path / "projectSettings.dproj").c_str();
            // size_t pos;
            /*while ((pos = projectSettingsPath.find('\\')) != std::string::npos) {
                projectSettingsPath.replace(pos, 1, "/");
            }*/

            if (FileHandler::CheckIfFileExists(projectSettingsPath))
            {
                // Update the projectSettings.dproj "projectName" entry
                // std::string templateProjectSettingsDirectory = templateProjectDirectory + "/projectSettings.dproj";
                std::string fileContent = FileHandler::ReadFile(projectSettingsPath);

                if (!fileContent.empty())
                {
                    nlohmann::json jsonObject = nlohmann::json::parse(fileContent);
                    if (jsonObject["projectInformation"]["graphicsApi"] != (int)api)
                    {
                        jsonObject["projectInformation"]["graphicsApi"] = (int)api;

                        std::string newFileContent = jsonObject.dump(4);
                        FileHandler::WriteToFile(projectSettingsPath, newFileContent);
                        s_ProjectList[id].graphicsApi = api;
                        SaveProjectList();
                    }
                }
                else
                {
                    std::cout << "[PROJECT LIST HANDLER] Error: Project settings file is empty" << std::endl;
                }
            }
            else
            {
                // wtf
                std::cout << "[PROJECT LIST HANDLER] Error: Project settings file not found" << std::endl;
            }
        }

        /// @brief Removes a project from the project list and saves the new list to the disk.
        /// @param id ID of the project to remove.
        static void RemoveProjectFromList(int id)
        {
            s_ProjectList.erase(s_ProjectList.begin() + id);
            SaveProjectList();
        }

        /// @brief This function is being called when a project is being opened. It updates the "last time opened" timestamp of the project.
        /// @param id ID of the project to be updated.
        static void RegisterProjectOpening(int id)
        {
            s_ProjectList[id].lastOpened = time(0);

            std::filesystem::path projectSettingsPath = (s_ProjectList[id].path / "projectSettings.dproj");
            if (FileHandler::CheckIfFileExists(projectSettingsPath))
            {
                // Update the projectSettings.dproj "projectName" entry
                // std::string templateProjectSettingsDirectory = templateProjectDirectory + "/projectSettings.sproj";
                std::string fileContent = FileHandler::ReadFile(projectSettingsPath);

                if (!fileContent.empty())
                {
                    nlohmann::json jsonObject = nlohmann::json::parse(fileContent);
                    jsonObject["projectInformation"]["lastOpened"] = time(0);

                    std::string newFileContent = jsonObject.dump(4);
                    FileHandler::WriteToFile(projectSettingsPath, newFileContent);
                }
                else
                {
                    std::cout << "[PROJECT LIST HANDLER] Error: Project settings file is empty" << std::endl;
                }
            }
            else
            {
                // wtf
                std::cout << "[PROJECT LIST HANDLER] Error: Project settings file not found" << std::endl;
            }

            SaveProjectList();
        }
    };
}