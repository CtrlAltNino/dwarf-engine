#include "ProjectCreator.h"
#include "Logging/IDwarfLogger.h"
#include <filesystem>

namespace Dwarf
{
  ProjectCreator::ProjectCreator(IDwarfLogger&                 logger,
                                 IProjectList&                 projectList,
                                 std::shared_ptr<IFileHandler> fileHandler)
    : m_Logger(logger)
    , m_ProjectList(projectList)
    , m_FileHandler(fileHandler)
  {
    m_Logger.LogDebug(Log("ProjectCreator created", "ProjectCreator"));
  }

  ProjectCreator::~ProjectCreator()
  {
    m_Logger.LogDebug(Log("ProjectCreator destroyed", "ProjectCreator"));
  }

  void
  ProjectCreator::CreateProject(std::string           projectName,
                                std::filesystem::path projectPath,
                                GraphicsApi           graphicsApi,
                                ProjectTemplate       projectTemplate)
  {
    m_Logger.LogInfo(Log(fmt::format("Creating project {} at {} ",
                                     projectName,
                                     projectPath.string()),
                         "ProjectCreator"));
    // Create Project
    std::filesystem::path projectDirectory = projectPath / projectName;
    if (!m_FileHandler->DirectoyExists(projectDirectory))
    {
      std::filesystem::path projectSettingsPath =
        projectPath / projectName / "projectSettings.dproj";
      if (!m_FileHandler->FileExists(projectSettingsPath))
      {
        m_Logger.LogDebug(
          Log("Creating project directory at: " + projectDirectory.string(),
              "ProjectCreator"));
        m_FileHandler->CreateDirectoryAt(projectDirectory);

        if (projectTemplate == ProjectTemplate::Blank)
        {
          std::filesystem::path projectSettingsPath =
            projectPath / projectName / "projectSettings.dproj";
          nlohmann::json jsonObject;
          jsonObject["projectInformation"]["projectName"] = projectName;
          jsonObject["projectInformation"]["graphicsApi"] = (int)graphicsApi;
          jsonObject["projectInformation"]["path"] =
            projectSettingsPath.string();

          std::string fileContent = jsonObject.dump(4);
          m_FileHandler->WriteToFile(projectSettingsPath, fileContent);
          m_Logger.LogDebug(Log("Project settings file created at: " +
                                  projectSettingsPath.string(),
                                "ProjectCreator"));
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

          m_Logger.LogDebug(Log("Project template copied to: " +
                                  (projectPath / projectName).string(),
                                "ProjectCreator"));

          // Update the projectSettings.dproj "projectName" entry
          std::filesystem::path templateProjectSettingsDirectory =
            templateProjectDirectory / "projectSettings.dproj";
          std::string fileContent =
            m_FileHandler->ReadFile(templateProjectSettingsDirectory);
          if (!fileContent.empty())
          {
            nlohmann::json jsonObject = nlohmann::json::parse(fileContent);
            jsonObject["projectInformation"]["projectName"] = projectName;

            std::string newFileContent = jsonObject.dump(4);
            m_FileHandler->WriteToFile(projectSettingsPath.c_str(),
                                       newFileContent);
            m_Logger.LogDebug(Log("Project settings file updated at: " +
                                    projectSettingsPath.string(),
                                  "ProjectCreator"));
          }
          else
          {
            m_Logger.LogWarn(
              Log("Failed to read template project settings file",
                  "ProjectCreator"));
          }
        }

        m_ProjectList.AddProject(projectDirectory);
        m_Logger.LogDebug(
          Log("Newly created project added to project list", "ProjectCreator"));
      }
      else
      {
        // Wtf how
        m_Logger.LogWarn(Log("Project settings file already exists at: " +
                               projectSettingsPath.string(),
                             "ProjectCreator"));
      }
    }
    else
    {
      m_Logger.LogWarn(
        Log("Project directory already exists at: " + projectDirectory.string(),
            "ProjectCreator"));
    }
  }

  std::filesystem::path
  ProjectCreator::GetDefaultProjectPath() const
  {
    return m_FileHandler->GetDocumentsPath();
  }
}
