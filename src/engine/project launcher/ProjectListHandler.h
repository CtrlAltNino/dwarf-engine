#pragma once

#include "../../utilities/dpch.h"

#include <nlohmann/json.hpp>
#include<nfd.h>

#include "../../utilities/FileHandler.h"
#include "ProjectLauncherUtilities.h"
#include "ProjectSorter.h"
#include "PathUtilities.h"

class ProjectListHandler {
    private:
        /// @brief Currently loaded list of informations regarding projects.
        static inline std::vector<ProjectInformation> projectList;

        /// @brief Saves the currently loaded list of project informations back to the disk.
        /// @return True if successful, false if not.
        static bool SaveProjectList(){
            bool sucess = true;
            nlohmann::json jsonObject;
            for (int i = 0; i < projectList.size(); i++) {
                jsonObject["projects"][i]["name"] = projectList[i].name;
                jsonObject["projects"][i]["path"] = projectList[i].path;
                jsonObject["projects"][i]["lastOpened"] = projectList[i].lastOpened;
                jsonObject["projects"][i]["graphicsApi"] = projectList[i].graphicsApi;
            }

            std::string fileContent = jsonObject.dump(4);
            std::string settingsPath = FileHandler::GetProjectSettingsPath();
            if (!FileHandler::checkIfDirectoyExists(settingsPath)) {
                FileHandler::createDirectoryS(settingsPath);
            }
            std::string savedProjectsPath = settingsPath + "/savedProjects.json";
            FileHandler::writeToFile(savedProjectsPath.c_str(), fileContent);
            return sucess;
        }
    public:
        /// @brief Returns the list of project informations.
        /// @return Pointer to the list of project informations.
        static std::vector<ProjectInformation>* GetProjectList(){
            return &projectList;
        }

        /// @brief Returns the project information of a given project.
        /// @param id ID of the project.
        /// @return Project information of the given project.
        static ProjectInformation GetProjectInformation(int id){
            ProjectInformation projectInformation;
            if(id < projectList.size()){
                projectInformation = projectList[id];
            }
            return projectInformation;
        }

        /// @brief Reads the project information from a given project path.
        /// @param path Path to the project.
        /// @return Project information of a project loaded from the disk.
        static ProjectInformation ExtractProjectInformation(const char* path){
            std::string projectSettingsPath = (std::string(path) + "/projectSettings.sproj");
            std::string fileContent = FileHandler::readFile(projectSettingsPath.c_str());
            
            ProjectInformation foundInfo;

            if (!fileContent.empty()) {
                nlohmann::json jsonObject = nlohmann::json::parse(fileContent);

                if (jsonObject.contains("projectInformation")) {
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
        static bool LoadProjectList(){
            bool success = true;
            std::string savedProjectsPath = FileHandler::GetProjectSettingsPath() + "/savedProjects.json";
            std::string fileContent = FileHandler::readFile(savedProjectsPath.c_str());
            if (!fileContent.empty()) {
                nlohmann::json jsonObject = nlohmann::json::parse(fileContent);

                if (jsonObject.contains("projects")) {
                    for (int i = 0; i < jsonObject["projects"].size(); i++) {
                        ProjectInformation projectToAdd;
                        if (jsonObject["projects"][i].contains("name")) {
                            projectToAdd.name = jsonObject["projects"][i]["name"];
                        }

                        if (jsonObject["projects"][i].contains("path")) {
                            projectToAdd.path = jsonObject["projects"][i]["path"];
                        }

                        if (jsonObject["projects"][i].contains("lastOpened")) {
                            projectToAdd.lastOpened = jsonObject["projects"][i]["lastOpened"];
                        }

                        if (jsonObject["projects"][i].contains("graphicsApi")) {
                            projectToAdd.graphicsApi = jsonObject["projects"][i]["graphicsApi"];
                        }

                        projectList.push_back({
                                projectToAdd
                            });
                    }

                    // After adding all the projects, sort them
                    ProjectSorter::SortProjectList(&projectList);
                }
            }
            return success;
        }

        /// @brief Adds a project to the project list and saves the new list to the disk.
        /// @param projectInformation The information of the project to add.
        /// @return True if project is already present in list, false if not.
        static bool AddProject(ProjectInformation projectInformation){
            bool alreadyPresent = false;
            for (int i = 0; (i < projectList.size()) && !alreadyPresent; i++) {
                alreadyPresent = projectList[i].path == projectInformation.path;
            }

            if (!alreadyPresent) {
                projectList.push_back(projectInformation);
                ProjectSorter::SortProjectList(&projectList);
                // Aktualisierte Projektliste speichern
                SaveProjectList();
            }

            return alreadyPresent;
        }

        /// @brief Adds a project to the project list and saves the new list to the disk.
        /// @param projectPath Path to the project to add.
        /// @return True if project is already present in list, false if not.
        static bool AddProjectByPath(const char* projectPath){
            return AddProject(ExtractProjectInformation(projectPath));
        }
        
        /// @brief Opens a native file dialog to add a project to the project list.
        static void OpenAddProjectWindow(){
            // File Dialog um einen Pfad zu kriegen
            // Im Pfad nach einer projectSettings.sproj suchen
            // Projectinformation (Name, Pfad, letzte Modifikationszeit)
            nfdchar_t* outPath = NULL;
            const nfdchar_t* filter = "sproj";

            // TODO implement file dialog in another thread to not interrupt
            // TODO rename "open" button to "add project" or something
            // TODO focus on the opened dialog when trying to return to the project launcher window
            //(if that is not the default behaviour, finding out when implementing the dialog in another thread)
            //nfdresult_t result = NFD_OpenDialog(filter, NULL, &outPath);
            nfdresult_t result = NFD_PickFolder(NULL, &outPath);
            
            if (result == NFD_OKAY) {
                // check if a project with the same path already exists
                bool alreadyPresent = false;
                outPath = PathUtilities::ConvertBackSlashesToForwardSlashes(outPath);

                for (int i = 0; (i < projectList.size()) && !alreadyPresent; i++) {
                    alreadyPresent = projectList[i].path == outPath;
                }
                
                if (!alreadyPresent) {
                    ProjectInformation newProject = ExtractProjectInformation(outPath);
                    if (newProject.name != "") {

                        projectList.push_back(newProject);
                        ProjectSorter::SortProjectList(&projectList);
                        // Aktualisierte Projektliste speichern
                        SaveProjectList();
                    }
                    else {
                        // TODO: Open modal to signal that no project could be found at the given path
                    }
                }
                else {
                    // TODO: Modal to notify the project is already present in the current project list
                }
                free(outPath);
            }
            else if (result == NFD_CANCEL) {
                //puts("User pressed cancel.");
            }
            else {
                //printf("Error: %s\n", NFD_GetError());
            }
        }

        /// @brief Changes the graphics API of a project.
        /// @param id Project ID.
        /// @param api Graphics API.
        static void ChangeGraphicsApi(int id, GraphicsApi api){
            //ProjectInformation info = ExtractProjectInformation(projectList[id].path.c_str());
            std::string projectSettingsPath = (projectList[id].path + "/projectSettings.sproj").c_str();
            //size_t pos;
            /*while ((pos = projectSettingsPath.find('\\')) != std::string::npos) {
                projectSettingsPath.replace(pos, 1, "/");
            }*/
            
            if (FileHandler::checkIfFileExists(projectSettingsPath.c_str())) {
                // Update the projectSettings.sproj "projectName" entry
                //std::string templateProjectSettingsDirectory = templateProjectDirectory + "/projectSettings.sproj";
                std::string fileContent = FileHandler::readFile(projectSettingsPath.c_str());

                if (!fileContent.empty()) {
                    nlohmann::json jsonObject = nlohmann::json::parse(fileContent);
                    if (jsonObject["projectInformation"]["graphicsApi"] != (int)api) {
                        jsonObject["projectInformation"]["graphicsApi"] = (int)api;

                        std::string newFileContent = jsonObject.dump(4);
                        FileHandler::writeToFile(projectSettingsPath.c_str(), newFileContent);
                        projectList[id].graphicsApi = (int)api;
                        SaveProjectList();
                    }
                }
                else {
                    std::cout << "yikes bro" << std::endl;
                }
            }
            else {
                // wtf
                std::cout << "There is no projectSettings.json file to be found for the mentioned project!" << std::endl;
            }
        }
        
        /// @brief Removes a project from the project list and saves the new list to the disk.
        /// @param id ID of the project to remove.
        static void RemoveProjectFromList(int id){
            projectList.erase(projectList.begin() + id);
            SaveProjectList();
        }

        /// @brief This function is being called when a project is being opened. It updates the "last time opened" timestamp of the project.
        /// @param id ID of the project to be updated.
        static void RegisterProjectOpening(int id){
            projectList[id].lastOpened = time(0);
            
            std::string projectSettingsPath = (projectList[id].path + "/projectSettings.sproj").c_str();
            if (FileHandler::checkIfFileExists(projectSettingsPath.c_str())) {
                // Update the projectSettings.sproj "projectName" entry
                //std::string templateProjectSettingsDirectory = templateProjectDirectory + "/projectSettings.sproj";
                std::string fileContent = FileHandler::readFile(projectSettingsPath.c_str());

                if (!fileContent.empty()) {
                    nlohmann::json jsonObject = nlohmann::json::parse(fileContent);
                    jsonObject["projectInformation"]["lastOpened"] = time(0);

                    std::string newFileContent = jsonObject.dump(4);
                    FileHandler::writeToFile(projectSettingsPath.c_str(), newFileContent);
                }
                else {
                    std::cout << "yikes bro" << std::endl;
                }
            }
            else {
                // wtf
                std::cout << "There is no projectSettings.json file to be found for the mentioned project!" << std::endl;
            }

            SaveProjectList();
        }
};