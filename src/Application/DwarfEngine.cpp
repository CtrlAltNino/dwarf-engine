#include "pch.h"

#include "Application/DwarfEngine.h"
#include "DI/Editor/EditorInjector.h"
#include "DI/ProjectLauncher/ProjectLauncherInjector.h"
#include "Editor/Editor.h"
#include "Launcher/ProjectLauncher.h"
#include <boost/di.hpp>

namespace Dwarf
{
  DwarfEngine::DwarfEngine()
    : mLogger(std::make_shared<DwarfLogger>(LogName("Application")))
  {
  }

  void
  DwarfEngine::Run()
  {
    bool                        returnToLauncher = true;
    std::optional<SavedProject> selectedProject = std::nullopt;

    while (returnToLauncher)
    {
      selectedProject = RunLauncher();

      if (selectedProject)
      {
        returnToLauncher = RunEditor(selectedProject.value());
        mLogger->LogInfo(Log("Editor finished running.", "DwarfEngine"));

        if (returnToLauncher)
        {
          mLogger->LogInfo(Log("Returning to launcher...", "DwarfEngine"));
        }
        else
        {
          mLogger->LogInfo(Log("Exiting Dwarf Engine...", "DwarfEngine"));
        }
      }
      else
      {
        mLogger->LogInfo(Log(
          "No project path provided. Exiting Dwarf Engine... ", "DwarfEngine"));
        returnToLauncher = false;
      }
    }
  }

  auto
  DwarfEngine::RunLauncher() -> std::optional<SavedProject>
  {
    std::optional<SavedProject> selectedProject = std::nullopt;

    mLogger->LogDebug(Log("Creating injector...", "DwarfEngine"));
    auto launcherInjector = ProjectLauncherInjector::createInjector();

    mLogger->LogDebug(Log("Creating project launcher...", "DwarfEngine"));
    auto launcher = launcherInjector.create<std::shared_ptr<ProjectLauncher>>();
    mLogger->LogDebug(Log("Project launcher created.", "DwarfEngine"));
    mLogger->LogDebug(Log("Running project launcher...", "DwarfEngine"));
    selectedProject = launcher->Run();
    mLogger->LogDebug(Log("Project launcher finished running.", "DwarfEngine"));

    return selectedProject;
  }

  auto
  DwarfEngine::RunEditor(const SavedProject& savedProject) -> bool
  {
    mLogger->LogDebug(
      Log("Opening project at: " + savedProject.Path.string(), "DwarfEngine"));
    mLogger->LogDebug(Log("Creating editor...", "DwarfEngine"));

    auto editorInjector = EditorInjector::createInjector(savedProject);

    auto editor = editorInjector.create<std::shared_ptr<Dwarf::Editor>>();
    bool returnToLauncher = editor->Run();

    mLogger->LogDebug(Log("Editor finished running.", "DwarfEngine"));
    mLogger->LogDebug(Log("Return to launcher: " +
                            std::to_string(static_cast<int>(returnToLauncher)),
                          "DwarfEngine"));

    return returnToLauncher;
  }
}
