#pragma once

#include "../../utilities/dpch.h"

#include <nlohmann/json.hpp>

#include"ProjectLauncherUtilities.h"
#include"ProjectListHandler.h"
#include "../../utilities/FileHandler.h"

/// @brief Class that provides project creation functionality.
class ProjectCreator {
    private:
        /// @brief The default path where projects should be created.
        static inline std::string defaultProjectPath;
    
    public:
        /// @brief Initialized the project creator.
        static void InitProjectCreator(){
            defaultProjectPath = FileHandler::GetDocumentsPath();
        }

        /// @brief Returns the default project path.
        /// @return Path to default directory to create projects in.
        static std::string GetDefaultProjectPath(){
            return defaultProjectPath;
        }
        
        /// @brief Creates a project.
        /// @param projectName Name of the project.
        /// @param projectPath Path where the project should be created at.
        /// @param graphicsApi API of the project.
        /// @param projectTemplate Which template should be used for creating the project.
        /// @return Error code.
        static int CreateProject(const char* projectName,
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
                        std::cout << (std::string(projectPath) + "/" + projectName) << std::endl;
                        FileHandler::writeToFile((std::string(projectPath) + "/" + projectName + "/projectSettings.sproj").c_str(), fileContent);
                    }
                    else {
                        std::string templateProjectDirectory = "";
                        std::string templateApiDirectory = "";

                        switch (projectTemplate) {
                        case ProjectTemplate::Demo1:
                            templateProjectDirectory = "demo1";
                            break;
                        case ProjectTemplate::Blank: break;
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
                        case GraphicsApi::Metal:
                            templateApiDirectory = "metal";
                            break;
                        }

                        templateProjectDirectory = "data/demo projects/" + templateApiDirectory + "/" + templateProjectDirectory;

                        #ifdef WIN32
                            std::string copyCommand = std::string("Xcopy \"" + templateProjectDirectory
                            + "\" \""
                            + std::string(projectPath) + "/" + projectName
                            + "\" /E/H/C/I/y/D");
                        #elif __APPLE__
                            std::string copyCommand = std::string("cp -R \"./") 
                                + templateProjectDirectory + "/\" \"" + std::string(projectPath) + "/" + projectName + "\"";
                        #endif
                        
                        system(copyCommand.c_str());
                        
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