#pragma once

#include <nlohmann/json.hpp>

#include "Launcher/ProjectLauncherUtilities.h"
#include "Launcher/ProjectListHandler.h"
#include "Utilities/FileHandler.h"

namespace Dwarf {

    /// @brief Class that provides project creation functionality.
    class ProjectCreator {
        private:
            /// @brief The default path where projects should be created.
            static inline std::filesystem::path s_DefaultProjectPath;
        
        public:
            /// @brief Initialized the project creator.
            static void InitProjectCreator(){
                s_DefaultProjectPath = FileHandler::GetDocumentsPath();
            }

            /// @brief Returns the default project path.
            /// @return Path to default directory to create projects in.
            static std::filesystem::path GetDefaultProjectPath(){
                return s_DefaultProjectPath;
            }
            
            /// @brief Creates a project.
            /// @param projectName Name of the project.
            /// @param projectPath Path where the project should be created at.
            /// @param graphicsApi API of the project.
            /// @param projectTemplate Which template should be used for creating the project.
            /// @return Error code.
            static int CreateProject(const char* projectName,
                                        std::filesystem::path projectPath,
                                        GraphicsApi graphicsApi,
                                        ProjectTemplate projectTemplate){
                // Create Project blabla
                std::filesystem::path projectDirectory = projectPath / projectName;
                if (!FileHandler::CheckIfDirectoyExists(projectDirectory)) {
                    std::filesystem::path projectSettingsPath = projectPath / projectName / "projectSettings.dproj";
                    if (!FileHandler::CheckIfFileExists(projectSettingsPath)) {
                        FileHandler::CreateDirectory(projectDirectory);

                        if (projectTemplate == ProjectTemplate::Blank) {
                            nlohmann::json jsonObject;
                            jsonObject["projectInformation"]["projectName"] = projectName;
                            jsonObject["projectInformation"]["graphicsApi"] = (int)graphicsApi;

                            std::string fileContent = jsonObject.dump(4);
                            FileHandler::WriteToFile(projectPath / projectName / "projectSettings.dproj", fileContent);
                        }
                        else {
                            std::filesystem::path templateProjectDirectory = "";
                            std::filesystem::path templateApiDirectory = "";

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

                            templateProjectDirectory = std::filesystem::path("data/demo projects") / templateApiDirectory / templateProjectDirectory;

                            #ifdef WIN32
                                std::string copyCommand = std::string("Xcopy \"" +
                                templateProjectDirectory.string() + "\" \"" + (projectPath / projectName).string() + "\" /E/H/C/I/y/D");
                            #elif __APPLE__
                                std::string copyCommand = std::string("cp -R \"./") 
                                    + templateProjectDirectory.string() + "/\" \"" + std::string(projectPath) + "/" + projectName + "\"";
                            #endif
                            
                            system(copyCommand.c_str());

                            // Update the projectSettings.dproj "projectName" entry
                            std::filesystem::path templateProjectSettingsDirectory = templateProjectDirectory / "projectSettings.dproj";
                            std::string fileContent = FileHandler::ReadFile(templateProjectSettingsDirectory);
                            if (!fileContent.empty()) {
                                nlohmann::json jsonObject = nlohmann::json::parse(fileContent);
                                jsonObject["projectInformation"]["projectName"] = projectName;

                                std::string newFileContent = jsonObject.dump(4);
                                FileHandler::WriteToFile(projectSettingsPath.c_str(), newFileContent);
                            }
                            else {
                                std::cout << "[PROJECT CREATOR] Could not read project settings" << std::endl;
                            }


                        }

                        ProjectListHandler::AddProjectByPath(projectDirectory.c_str());

                        return 0;
                    }
                    else {
                        // wtf
                        std::cout << "[PROJECT CREATOR] projectSettings.dproj already exists at project directory!" << std::endl;
                        return 1;
                    }
                }
                else {
                    // Project folder already exists
                    std::cout << "[PROJECT CREATOR] Project folder already exists at: " << (projectPath / projectName)<< std::endl;
                    return 2;
                }
            }
    };
}