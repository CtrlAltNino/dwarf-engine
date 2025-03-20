#include "ProjectCreator.h"
#include "Core/Base.h"
#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettings.h"
#include <filesystem>
#include <fmt/format.h>

namespace Dwarf
{
  ProjectCreator::ProjectCreator(
    std::shared_ptr<IDwarfLogger>       logger,
    std::shared_ptr<ISavedProjects>     savedProjects,
    std::shared_ptr<IFileHandler>       fileHandler,
    std::shared_ptr<IProjectSettingsIO> projectSettingsIO)
    : mLogger(logger)
    , mSavedProjects(savedProjects)
    , mFileHandler(fileHandler)
    , mProjectSettingsIO(projectSettingsIO)
  {
    mLogger->LogDebug(Log("ProjectCreator created", "ProjectCreator"));
  }

  ProjectCreator::~ProjectCreator()
  {
    mLogger->LogDebug(Log("ProjectCreator destroyed", "ProjectCreator"));
  }

  void
  ProjectCreator::CreateProject(std::string           projectName,
                                std::filesystem::path projectPath,
                                GraphicsApi           graphicsApi,
                                ProjectTemplate       projectTemplate)
  {
    mLogger->LogInfo(Log(fmt::format("Creating project {} at {} ",
                                     projectName,
                                     projectPath.string()),
                         "ProjectCreator"));
    // Create Project
    std::filesystem::path projectDirectory = projectPath / projectName;
    if (!mFileHandler->DirectoryExists(projectDirectory))
    {
      if (!mFileHandler->FileExists(projectDirectory / "projectSettings.dproj"))
      {
        mLogger->LogDebug(
          Log("Creating project directory at: " + projectDirectory.string(),
              "ProjectCreator"));
        mFileHandler->CreateDirectoryAt(projectDirectory);

        if (projectTemplate == ProjectTemplate::Blank)
        {
          ProjectSettingsData projectSettingsData;
          projectSettingsData.ProjectName = projectName;
          projectSettingsData.GraphicsApi = graphicsApi;

          mProjectSettingsIO->SaveProjectSettings(projectSettingsData,
                                                  projectDirectory);

          mLogger->LogDebug(Log("Project settings file created at: " +
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
            case Dwarf::ProjectTemplate::DamagedHelmet:
              templateProjectDirectory = "damaged helmet";
            case ProjectTemplate::Blank: break;
          }

          switch (graphicsApi)
          {
            using enum GraphicsApi;
            case None:
              std::runtime_error(
                "No Graphics API selected for creating a project");
              break;
            case D3D12: templateApiDirectory = "dx12"; break;
            case Metal: templateApiDirectory = "metal"; break;
            case OpenGL: templateApiDirectory = "opengl"; break;
            case Vulkan: templateApiDirectory = "vulkan"; break;
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

          mLogger->LogDebug(Log("Project template copied to: " +
                                  (projectPath / projectName).string(),
                                "ProjectCreator"));

          std::optional<ProjectSettingsData> projectSettingsData =
            mProjectSettingsIO->LoadProjectSettings(projectDirectory);

          if (projectSettingsData)
          {
            projectSettingsData->ProjectName = projectName;
            projectSettingsData->GraphicsApi = graphicsApi;

            mProjectSettingsIO->SaveProjectSettings(projectSettingsData.value(),
                                                    projectDirectory);
            mLogger->LogDebug(Log("Project settings file updated at: " +
                                    projectDirectory.string(),
                                  "ProjectCreator"));
          }
          else
          {
            mLogger->LogError(Log("Failed to find copied project settings file",
                                  "ProjectCreator"));
          }
        }

        mSavedProjects->AddProject(projectDirectory);
        mLogger->LogDebug(
          Log("Newly created project added to project list", "ProjectCreator"));
      }
      else
      {
        // Wtf how
        mLogger->LogWarn(Log("Project settings file already exists at: " +
                               projectDirectory.string(),
                             "ProjectCreator"));
      }
    }
    else
    {
      mLogger->LogWarn(
        Log("Project directory already exists at: " + projectDirectory.string(),
            "ProjectCreator"));
    }
  }

  std::filesystem::path
  ProjectCreator::GetDefaultProjectPath() const
  {
    return mFileHandler->GetDocumentsPath();
  }
}
