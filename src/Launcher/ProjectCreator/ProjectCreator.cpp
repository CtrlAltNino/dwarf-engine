#include "ProjectCreator.h"
#include "Utilities/FileHandler.h"

namespace Dwarf
{
  ProjectCreator::ProjectCreator(std::shared_ptr<IProjectList> projectList)
    : m_ProjectList(projectList)
  {
  }

  void
  ProjectCreator::CreateProject(std::string           projectName,
                                std::filesystem::path projectPath,
                                GraphicsApi           graphicsApi,
                                ProjectTemplate       projectTemplate)
  {
    // Create Project blabla
    std::filesystem::path projectDirectory = projectPath / projectName;
    if (!FileHandler::DirectoyExists(projectDirectory))
    {
      std::filesystem::path projectSettingsPath =
        projectPath / projectName / "projectSettings.dproj";
      if (!FileHandler::FileExists(projectSettingsPath))
      {
        FileHandler::CreateDirectoryAt(projectDirectory);

        if (projectTemplate == ProjectTemplate::Blank)
        {
          nlohmann::json jsonObject;
          jsonObject["projectInformation"]["projectName"] = projectName;
          jsonObject["projectInformation"]["graphicsApi"] = (int)graphicsApi;

          std::string fileContent = jsonObject.dump(4);
          FileHandler::WriteToFile(
            projectPath / projectName / "projectSettings.dproj", fileContent);
        }
        else
        {
          std::filesystem::path templateProjectDirectory = "";
          std::filesystem::path templateApiDirectory = "";

          switch (projectTemplate)
          {
            case ProjectTemplate::Sponza:
              templateProjectDirectory = "sponza";
              break;
            case ProjectTemplate::Blank: break;
          }

          switch (graphicsApi)
          {
            case GraphicsApi::D3D12: templateApiDirectory = "dx12"; break;
            case GraphicsApi::Metal: templateApiDirectory = "metal"; break;
            case GraphicsApi::OpenGL: templateApiDirectory = "opengl"; break;
            case GraphicsApi::Vulkan: templateApiDirectory = "vulkan"; break;
          }

          templateProjectDirectory =
            std::filesystem::path("data/demo projects") / templateApiDirectory /
            templateProjectDirectory;
          std::string copyCommand = "";

#ifdef _WIN32
          copyCommand = std::string(
            "Xcopy \"" + templateProjectDirectory.string() + "\" \"" +
            (projectPath / projectName).string() + "\" /E/H/C/I/y/D");
#elif __APPLE__
          copyCommand =
            std::string("cp \"" + templateProjectDirectory.string() + "\" \"" +
                        (projectPath / projectName).string() + "\"");
#elif __linux__
          copyCommand = std::string(
            "cp -a \"" + templateProjectDirectory.string() + "/.\" \"" +
            (projectPath / projectName).string() + "\"");
#endif
          system(copyCommand.c_str());

          // Update the projectSettings.dproj "projectName" entry
          std::filesystem::path templateProjectSettingsDirectory =
            templateProjectDirectory / "projectSettings.dproj";
          std::string fileContent =
            FileHandler::ReadFile(templateProjectSettingsDirectory);
          if (!fileContent.empty())
          {
            nlohmann::json jsonObject = nlohmann::json::parse(fileContent);
            jsonObject["projectInformation"]["projectName"] = projectName;

            std::string newFileContent = jsonObject.dump(4);
            FileHandler::WriteToFile(projectSettingsPath.c_str(),
                                     newFileContent);
          }
          else
          {
            std::cout << "[PROJECT CREATOR] Could not read project settings"
                      << std::endl;
          }
        }

        m_ProjectList->AddProject(projectDirectory);
      }
      else
      {
        // wtf
        std::cout << "[PROJECT CREATOR] projectSettings.dproj already exists "
                     "at project directory!"
                  << std::endl;
      }
    }
    else
    {
      // Project folder already exists
      std::cout << "[PROJECT CREATOR] Project folder already exists at: "
                << (projectPath / projectName) << std::endl;
    }
  }

  std::filesystem::path
  ProjectCreator::GetDefaultProjectPath() const
  {
    return FileHandler::GetDocumentsPath();
  }
}
