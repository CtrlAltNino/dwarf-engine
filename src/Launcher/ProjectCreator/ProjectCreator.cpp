#include "pch.h"

#include "Core/Base.h"
#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/SavedProjects/ISavedProjects.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettings.h"
#include "ProjectCreator.h"

namespace Dwarf
{
  ProjectCreator::ProjectCreator(
    std::shared_ptr<IDwarfLogger>       logger,
    std::shared_ptr<ISavedProjects>     savedProjects,
    std::shared_ptr<IFileHandler>       fileHandler,
    std::shared_ptr<IProjectSettingsIO> projectSettingsIO)
    : mLogger(std::move(logger))
    , mSavedProjects(std::move(savedProjects))
    , mFileHandler(std::move(fileHandler))
    , mProjectSettingsIO(std::move(projectSettingsIO))
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

        std::filesystem::path templateProjectDirectory = "";
        std::filesystem::path templateApiDirectory = "";

        switch (projectTemplate)
        {
          case ProjectTemplate::Blank:
            templateProjectDirectory = "blank";
            break;
          case ProjectTemplate::Olaf: templateProjectDirectory = "olaf"; break;
          case ProjectTemplate::CrytekSponza:
            templateProjectDirectory = "crytek sponza";
            break;
          case Dwarf::ProjectTemplate::ScifiHelmet:
            templateProjectDirectory = "sci-fi helmet";
        }

        switch (graphicsApi)
        {
          using enum GraphicsApi;
          case None:
            mLogger->LogError(
              Log("Graphics API is not set", "ShaderAssetSelectorFactory"));
            throw std::runtime_error("Graphics API is not set");
          case OpenGL: templateApiDirectory = "opengl"; break;
          case Vulkan:
            mLogger->LogError(Log("Vulkan API has not been implemented yet",
                                  "ShaderAssetSelectorFactory"));
            throw std::runtime_error("Vulkan API has not been implemented yet");
          case D3D12:
#ifdef _WIN32
            mLogger->LogError(Log("Direct3D12 API has not been implemented yet",
                                  "ShaderAssetSelectorFactory"));
            throw std::runtime_error(
              "Direct3D12 API has not been implemented yet");
#elif __linux__
            mLogger->LogError(Log("Direct3D12 is only supported on Windows",
                                  "ShaderAssetSelectorFactory"));
            throw std::runtime_error("Direct3D12 is only supported on Windows");
#endif
        }

        templateProjectDirectory = std::filesystem::path("data/templates") /
                                   templateApiDirectory /
                                   templateProjectDirectory;
        std::string copyCommand;

#ifdef _WIN32
        copyCommand =
          std::string("Xcopy \"" + templateProjectDirectory.string() + "\" \"" +
                      (projectPath / projectName).string() + "\" /E/H/C/I/y/D");
#elif __APPLE__
        copyCommand =
          std::string("cp \"" + templateProjectDirectory.string() + "\" \"" +
                      (projectPath / projectName).string() + "\"");
#elif __linux__
        copyCommand =
          std::string("cp -a \"" + templateProjectDirectory.string() +
                      "/.\" \"" + (projectPath / projectName).string() + "\"");
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

  auto
  ProjectCreator::GetDefaultProjectPath() const -> std::filesystem::path
  {
    return mFileHandler->GetDocumentsPath();
  }
}
