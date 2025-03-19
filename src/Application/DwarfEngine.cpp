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
    : m_Logger(std::make_shared<DwarfLogger>(LogName("Application")))
  {
  }

  void
  DwarfEngine::run()
  {
    bool                        returnToLauncher = true;
    std::optional<SavedProject> selectedProject = std::nullopt;

    while (returnToLauncher)
    {
      selectedProject = runLauncher();

      if (selectedProject)
      {
        returnToLauncher = runEditor(selectedProject.value());
        m_Logger->LogInfo(Log("Editor finished running.", "DwarfEngine"));

        if (returnToLauncher)
        {
          m_Logger->LogInfo(Log("Returning to launcher...", "DwarfEngine"));
        }
        else
        {
          m_Logger->LogInfo(Log("Exiting Dwarf Engine...", "DwarfEngine"));
        }
      }
      else
      {
        m_Logger->LogInfo(Log(
          "No project path provided. Exiting Dwarf Engine... ", "DwarfEngine"));
        returnToLauncher = false;
      }
    }
  }

  auto
  DwarfEngine::runLauncher() -> std::optional<SavedProject>
  {
    std::optional<SavedProject> selectedProject = std::nullopt;

    m_Logger->LogDebug(Log("Creating injector...", "DwarfEngine"));
    auto launcherInjector = ProjectLauncherInjector::createInjector();

    m_Logger->LogDebug(Log("Creating project launcher...", "DwarfEngine"));
    auto launcher = launcherInjector.create<std::shared_ptr<ProjectLauncher>>();
    m_Logger->LogDebug(Log("Project launcher created.", "DwarfEngine"));
    m_Logger->LogDebug(Log("Running project launcher...", "DwarfEngine"));
    selectedProject = launcher->Run();
    m_Logger->LogDebug(
      Log("Project launcher finished running.", "DwarfEngine"));

    return selectedProject;
  }

  auto
  DwarfEngine::runEditor(const SavedProject& savedProject) -> bool
  {
    m_Logger->LogDebug(
      Log("Opening project at: " + savedProject.Path.string(), "DwarfEngine"));
    m_Logger->LogDebug(Log("Creating editor...", "DwarfEngine"));

    auto editorInjector = EditorInjector::createInjector(savedProject);

    auto editor = editorInjector.create<std::shared_ptr<Dwarf::Editor>>();
    bool returnToLauncher = editor->Run();

    m_Logger->LogDebug(Log("Editor finished running.", "DwarfEngine"));
    m_Logger->LogDebug(Log("Return to launcher: " +
                             std::to_string(static_cast<int>(returnToLauncher)),
                           "DwarfEngine"));

    return returnToLauncher;
  }
}
