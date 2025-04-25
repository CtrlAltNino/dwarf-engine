#include "pch.hpp"

#include "IO/ISavedProjectsIO.hpp"
#include "ISavedProjects.hpp"
#include "Project/IProjectSettings.hpp"
#include "SavedProjects.hpp"
#include <nfd.hpp>

namespace Dwarf
{
  SavedProjects::SavedProjects(
    std::shared_ptr<IDwarfLogger>         logger,
    std::shared_ptr<ISavedProjectsIO>     savedProjectsIO,
    std::shared_ptr<ISavedProjectsSorter> savedProjectsSorter,
    std::shared_ptr<IProjectSettingsIO>   projectSettingsIO)
    : mLogger(std::move(logger))
    , mSavedProjectsIO(std::move(savedProjectsIO))
    , mSavedProjectsSorter(std::move(savedProjectsSorter))
    , mProjectSettingsIO(std::move(projectSettingsIO))
  {
    mLogger->LogInfo(Log("Loading SavedProjects...", "SavedProjects"));
    mSavedProjects = mSavedProjectsIO->LoadSavedProjects();
    mLogger->LogInfo(Log("SavedProjects loaded.", "SavedProjects"));

    mLogger->LogDebug(Log("Sorting SavedProjects...", "SavedProjects"));
    mSavedProjectsSorter->SortSavedProjects(mSavedProjects);
    mLogger->LogDebug(Log("SavedProjects sorted.", "SavedProjects"));

    mLogger->LogDebug(Log("SavedProjects created.", "SavedProjects"));
    NFD_Init();
    mLogger->LogDebug(Log("Native File Dialog initialized.", "SavedProjects"));
  }

  SavedProjects::~SavedProjects()
  {
    mLogger->LogInfo(Log("Destroying SavedProjects...", "SavedProjects"));
    NFD_Quit();
    mLogger->LogInfo(Log("Native File Dialog destroyed.", "SavedProjects"));
  }

  void
  SavedProjects::AddProject(const std::filesystem::path& path)
  {
    mLogger->LogDebug(Log("Adding project...", "SavedProjects"));
    SavedProject info;
    info.Path = path;
    info.ProjectName = path.filename().string();
    info.LastOpened = 0;
    info.Api = GraphicsApi::OpenGL;
    mSavedProjects.push_back(info);
    mSavedProjectsIO->SaveSavedProjects(mSavedProjects);
    mLogger->LogDebug(Log("Project added.", "SavedProjects"));
  }

  void
  SavedProjects::OpenAddProjectDialog()
  {
    // TODO: implement file dialog in another thread to not interrupt
    // TODO: rename "open" button to "add project" or something
    // TODO: focus on the opened dialog when trying to return to the project
    mLogger->LogInfo(Log("Opening Add Project Dialog...", "SavedProjects"));

    NFD::Guard nfdGuard;
    // auto-freeing memory
    NFD::UniquePath outPath;
    // show the dialog
    nfdresult_t result = NFD::PickFolder(outPath);

    if (result == NFD_OKAY)
    {
      mLogger->LogInfo(
        Log("Selected path: " + std::string(outPath.get()), "SavedProjects"));
      // check if a project with the same path already exists
      bool alreadyPresent = false;

      for (int i = 0; (i < mSavedProjects.size()) && !alreadyPresent; i++)
      {
        alreadyPresent = mSavedProjects[i].Path == outPath.get();
      }

      if (!alreadyPresent)
      {
        std::optional<ProjectSettingsData> newProject =
          mProjectSettingsIO->LoadProjectSettings(outPath.get());
        if (newProject)
        {
          SavedProject newSavedProject;
          newSavedProject.ProjectName = newProject->ProjectName;
          newSavedProject.Api = newProject->Api;
          newSavedProject.LastOpened = 0;
          newSavedProject.Path = outPath.get();

          mSavedProjects.push_back(newSavedProject);
          mLogger->LogInfo(
            Log("Project added to project list.", "SavedProjects"));

          mSavedProjectsSorter->SortSavedProjects(mSavedProjects);
          mLogger->LogDebug(Log("Project list sorted.", "SavedProjects"));

          mSavedProjectsIO->SaveSavedProjects(mSavedProjects);
          mLogger->LogDebug(Log("Project list saved.", "SavedProjects"));
        }
        else
        {
          // TODO: Open modal to signal that no project could be found at the
          // given path
          mLogger->LogWarn(
            Log("No project found at the given path.", "SavedProjects"));
        }
      }
      else
      {
        // TODO: Modal to notify the project is already present in the current
        // project list
        mLogger->LogWarn(
          Log("Project already present in project list.", "SavedProjects"));
      }
    }
    else if (result == NFD_CANCEL)
    {
      mLogger->LogInfo(Log("User pressed cancel.", "SavedProjects"));
    }
    else
    {
      mLogger->LogError(
        Log(fmt::format("Error: {}", std::string(NFD_GetError())),
            "SavedProjects"));
    }
  }

  void
  SavedProjects::RemoveProject(const std::filesystem::path& path)
  {
    mLogger->LogInfo(Log("Removing project...", "SavedProjects"));
    for (auto it = mSavedProjects.begin(); it != mSavedProjects.end(); ++it)
    {
      if (it->Path == path)
      {
        mLogger->LogInfo(Log("Project removed.", "SavedProjects"));
        mSavedProjects.erase(it);
        mSavedProjectsIO->SaveSavedProjects(mSavedProjects);
        return;
      }
    }

    mLogger->LogWarn(Log("Project to remove not found.", "SavedProjects"));
  }

  void
  SavedProjects::ChangeGraphicsApi(const std::filesystem::path& path,
                                   GraphicsApi                  api)
  {
    mLogger->LogDebug(Log("Changing graphics API...", "SavedProjects"));
    for (auto& project : mSavedProjects)
    {
      if (project.Path == path)
      {
        project.Api = api;
        mLogger->LogInfo(Log("Graphics API changed.", "SavedProjects"));
        mSavedProjectsSorter->SortSavedProjects(mSavedProjects);
        mSavedProjectsIO->SaveSavedProjects(mSavedProjects);
        mLogger->LogDebug(Log("Project list saved.", "SavedProjects"));
        break;
      }

      if (&project == &mSavedProjects.back())
      {
        mLogger->LogWarn(Log("Project to change not found.", "SavedProjects"));
      }
    }
  }

  void
  SavedProjects::RegisterProjectOpening(const std::filesystem::path& path)
  {
    mLogger->LogInfo(Log("Registering project opening...", "SavedProjects"));
    for (auto& project : mSavedProjects)
    {
      mLogger->LogInfo(Log("Checking project...", "SavedProjects"));
      if (project.Path == path)
      {
        project.LastOpened = time(nullptr);
        mLogger->LogInfo(Log("Project opening registered.", "SavedProjects"));
        mSavedProjectsSorter->SortSavedProjects(mSavedProjects);
        mLogger->LogInfo(Log("Project list sorted.", "SavedProjects"));
        mSavedProjectsIO->SaveSavedProjects(mSavedProjects);
        mLogger->LogInfo(Log("Project list saved.", "SavedProjects"));
        break;
      }

      if (&project == &mSavedProjects.back())
      {
        mLogger->LogWarn(
          Log("Project to register new launch not found.", "SavedProjects"));
      }
    }
  }

  auto
  SavedProjects::GetSavedProjects() -> std::vector<SavedProject>&
  {
    return mSavedProjects;
  }
}