#pragma once

#include<string>
#include <nlohmann/json.hpp>

#include"ProjectLauncherUtilities.h"
#include"ProjectListHandler.h"
#include "../../utilities/FileHandler.h"

class ProjectCreator {
    private:
        static inline std::string defaultProjectPath;
    
    public:
        static void InitProjectCreator(){
            defaultProjectPath = FileHandler::CreateDefaultProjectPath();
        }

        static std::string GetDefaultProjectPath(){
            return defaultProjectPath;
        }
        
        static int ProjectCreator::CreateProject(const char* projectName,
                                    const char* projectPath,
                                    GraphicsApi graphicsApi,
                                    ProjectTemplate projectTemplate){
            // Create Project blabla
            std::string projectDirectory = std::string(projectPath) + "/" + projectName;
            if (!FileHandler::checkIfDirectoyExists(projectDirectory)) {
                std::string projectSettingsPath = (std::string(projectPath) + "/" + projectName + "/projectSettings.sproj").c_str();
                size_t pos;
                while ((pos = projectSettingsPath.find('\\')) != std::string::npos) {
                    projectSettingsPath.replace(pos, 1, "/");
                }
                if (!FileHandler::checkIfFileExists(projectSettingsPath.c_str())) {
                    FileHandler::createDirectoryS(projectDirectory);

                    if (projectTemplate == ProjectTemplate::Blank) {
                        nlohmann::json jsonObject;
                        jsonObject["projectInformation"]["projectName"] = projectName;
                        jsonObject["projectInformation"]["graphicsApi"] = (int)graphicsApi;

                        std::string fileContent = jsonObject.dump(4);
                        FileHandler::writeToFile((std::string(projectPath) + "/" + projectName + "/projectSettings.sproj").c_str(), fileContent);
                    }
                    else {
                        std::string templateProjectDirectory = "";
                        std::string templateApiDirectory = "";

                        switch (projectTemplate) {
                        case ProjectTemplate::Demo1:
                            templateProjectDirectory = "demo1";
                            break;
                        }

                        switch (graphicsApi) {
                        case GraphicsApi::OpenGL:
                            templateApiDirectory = "opengl";
                            break;
                        case GraphicsApi::D3D11:
                            templateApiDirectory = "dx11";
                            break;
                        case GraphicsApi::D3D12:
                            templateApiDirectory = "dx12";
                            break;
                        case GraphicsApi::Vulkan:
                            templateApiDirectory = "vulkan";
                            break;
                        }

                        templateProjectDirectory = "data/demo projects/" + templateApiDirectory + "/" + templateProjectDirectory;

                        std::string copyCommand = std::string("Xcopy \"" + templateProjectDirectory
                            + "\" \""
                            + std::string(projectPath) + "/" + projectName
                            + "\" /E/H/C/I/y/D");
                        
                        //std::cout << copyCommand << std::endl;

                        // Update the projectSettings.sproj "projectName" entry
                        std::string templateProjectSettingsDirectory = templateProjectDirectory + "/projectSettings.sproj";
                        std::string fileContent = FileHandler::readFile(templateProjectSettingsDirectory.c_str());

                        if (!fileContent.empty()) {
                            nlohmann::json jsonObject = nlohmann::json::parse(fileContent);
                            jsonObject["projectInformation"]["projectName"] = projectName;

                            std::string newFileContent = jsonObject.dump(4);
                            FileHandler::writeToFile(projectSettingsPath.c_str(), newFileContent);
                        }
                        else {
                            std::cout << "yikes bro" << std::endl;
                        }


                        system(copyCommand.c_str());
                    }

                    size_t pos;
                    while ((pos = projectDirectory.find('\\')) != std::string::npos) {
                        projectDirectory.replace(pos, 1, "/");
                    }

                    /*ProjectInformation newProjectInformation;
                    newProjectInformation.name = projectName;
                    newProjectInformation.path = projectDirectory;
                    newProjectInformation.lastOpened = -1;
                    newProjectInformation.graphicsApi = (int)graphicsApi;

                    AddProjectToList(newProjectInformation);*/
                    std::cout << "Added project path: " << projectDirectory.c_str() << std::endl;
                    ProjectListHandler::AddProjectByPath(projectDirectory.c_str());

                    return 0;
                }
                else {
                    // wtf
                    std::cout << "projectSettings.json already exists at project directory!" << std::endl;
                    return 1;
                }
            }
            else {
                // Project folder already exists
                std::cout << "Project folder already exists at: " << (std::string(projectPath) + "/" + projectName) << std::endl;
                return 2;
            }
        }
};