#include "ProjectCreator.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettings.h"
#include <filesystem>

namespace Dwarf
{
  ProjectCreator::ProjectCreator(
    std::shared_ptr<IDwarfLogger>       logger,
    std::shared_ptr<ISavedProjects>     savedProjects,
    std::shared_ptr<IFileHandler>       fileHandler,
    std::shared_ptr<IProjectSettingsIO> projectSettingsIO)
    : m_Logger(logger)
    , m_SavedProjects(savedProjects)
    , m_FileHandler(fileHandler)
    , m_ProjectSettingsIO(projectSettingsIO)
  {
    m_Logger->LogDebug(Log("ProjectCreator created", "ProjectCreator"));
  }

  ProjectCreator::~ProjectCreator()
  {
    m_Logger->LogDebug(Log("ProjectCreator destroyed", "ProjectCreator"));
  }

  void
  ProjectCreator::CreateProject(std::string           projectName,
                                std::filesystem::path projectPath,
                                GraphicsApi           graphicsApi,
                                ProjectTemplate       projectTemplate)
  {
    m_Logger->LogInfo(Log(fmt::format("Creating project {} at {} ",
                                      projectName,
                                      projectPath.string()),
                          "ProjectCreator"));
    // Create Project
    std::filesystem::path projectDirectory = projectPath / projectName;
    if (!m_FileHandler->DirectoyExists(projectDirectory))
    {
      if (!m_FileHandler->FileExists(projectDirectory /
                                     "projectSettings.dproj"))
      {
        m_Logger->LogDebug(
          Log("Creating project directory at: " + projectDirectory.string(),
              "ProjectCreator"));
        m_FileHandler->CreateDirectoryAt(projectDirectory);

        if (projectTemplate == ProjectTemplate::Blank)
        {
          ProjectSettingsData projectSettingsData;
          projectSettingsData.ProjectName = projectName;
          projectSettingsData.GraphicsApi = graphicsApi;

          m_ProjectSettingsIO->SaveProjectSettings(projectSettingsData,
                                                   projectDirectory);

          m_Logger->LogDebug(Log("Project settings file created at: " +
                                   projectDirectory.string(),
                                 "ProjectCreator"));
        }
        else
        {
          std::filesystem::path templateProjectDirectory = "";
          std::filesystem::path templateApiDirectory = "";

          switch (projectTemplate)
          {
            case ProjectTemplate::Olaf:
              templateProjectDirectory = "olaf";
              break;
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

          m_Logger->LogDebug(Log("Project template copied to: " +
                                   (projectPath / projectName).string(),
                                 "ProjectCreator"));

          std::optional<ProjectSettingsData> projectSettingsData =
            m_ProjectSettingsIO->LoadProjectSettings(projectDirectory);

          if (projectSettingsData)
          {
            projectSettingsData->ProjectName = projectName;
            projectSettingsData->GraphicsApi = graphicsApi;

            m_ProjectSettingsIO->SaveProjectSettings(
              projectSettingsData.value(), projectDirectory);
            m_Logger->LogDebug(Log("Project settings file updated at: " +
                                     projectDirectory.string(),
                                   "ProjectCreator"));
          }
          else
          {
            m_Logger->LogError(Log(
              "Failed to find copied project settings file", "ProjectCreator"));
          }
        }

        m_SavedProjects->AddProject(projectDirectory);
        m_Logger->LogDebug(
          Log("Newly created project added to project list", "ProjectCreator"));
      }
      else
      {
        // Wtf how
        m_Logger->LogWarn(Log("Project settings file already exists at: " +
                                projectDirectory.string(),
                              "ProjectCreator"));
      }
    }
    else
    {
      m_Logger->LogWarn(
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
