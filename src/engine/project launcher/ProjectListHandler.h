#pragma once

#include <vector>

#include <nlohmann/json.hpp>
#include<nfd.h>

#include "../../utilities/FileHandler.h"
#include "ProjectLauncherUtilities.h"
#include "ProjectSorter.h"
#include "PathUtilities.h"

class ProjectListHandler {
    private:
        static inline std::vector<ProjectInformation> projectList;

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
            std::string settingsPath = GetSettingsPath();
            if (!FileHandler::checkIfDirectoyExists(settingsPath)) {
                FileHandler::createDirectoryS(GetSettingsPath());
            }
            std::string savedProjectsPath = settingsPath + "savedProjects.json";
            //std::cout << "Saving savedProjects.json to: " << GetSettingsPath() << std::endl;
            FileHandler::writeToFile(savedProjectsPath.c_str(), fileContent);
            return sucess;
        }

        static std::string GetSettingsPath() {
            std::string savedProjectsPath = "";

            #if _WIN32
            static char str[128];
            
            // Get Documents directory path
            {
                PWSTR path = NULL;
                HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &path);

                if (SUCCEEDED(hr)) {
                    size_t i;
                    wcstombs_s(&i, str, (size_t)128, path, (size_t)127);
                    savedProjectsPath = std::string((const char*)str);
                }

                CoTaskMemFree(path);
            }
            #endif
            
            if (savedProjectsPath != "") {
                size_t pos;
                while ((pos = savedProjectsPath.find('\\')) != std::string::npos) {
                    savedProjectsPath.replace(pos, 1, "/");
                }
                savedProjectsPath += "/Dwarf Engine/settings/";
            }
            
            //return "C:/Users/ninom/Documents/yeet";
            return savedProjectsPath;
        }
    
    public:
        static std::vector<ProjectInformation> GetProjectList(){
            return projectList;
        }

        static ProjectInformation GetProjectInformation(int id){
            ProjectInformation projectInformation;
            if(id < projectList.size()){
                projectInformation = projectList[id];
            }
            return projectInformation;
        }

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
                    foundInfo.lastOpened = jsonObject["projectInformation"]["lastOpened"];
                }
            }

            return foundInfo;
        }

        static bool LoadProjectList(){
            bool success = true;
            std::string savedProjectsPath = GetSettingsPath() + "savedProjects.json";
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
                    ProjectSorter::SortProjectList(projectList);
                }
            }
            return success;
        }

        static bool AddProject(ProjectInformation projectInformation){
            bool alreadyPresent = false;
            for (int i = 0; (i < projectList.size()) && !alreadyPresent; i++) {
                alreadyPresent = projectList[i].path == projectInformation.path;
            }

            if (!alreadyPresent) {
                projectList.push_back(projectInformation);
                ProjectSorter::SortProjectList(projectList);
                // Aktualisierte Projektliste speichern
                SaveProjectList();
            }

            return alreadyPresent;
        }

        static bool AddProjectByPath(const char* projectPath){
            return AddProject(ExtractProjectInformation(projectPath));
        }

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
                        ProjectSorter::SortProjectList(projectList);
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

        static void RemoveProjectFromList(int id){
            projectList.erase(projectList.begin() + id);
            SaveProjectList();
        }

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