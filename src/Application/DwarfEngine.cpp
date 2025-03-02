#include "pch.h"

#include "Application/DwarfEngine.h"
#include <boost/di.hpp>
#include "Launcher/ProjectLauncher.h"
#include "Editor/Editor.h"
#include "DI/ProjectLauncher/ProjectLauncherInjector.h"
#include "DI/Editor/EditorInjector.h"

namespace Dwarf
{
  DwarfEngine::DwarfEngine()
    : m_Logger(std::make_shared<DwarfLogger>(LogName("Application")))
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

  std::optional<SavedProject>
  DwarfEngine::RunLauncher()
  {
    std::optional<SavedProject> selectedProject = std::nullopt;

    m_Logger->LogDebug(Log("Creating injector...", "DwarfEngine"));
    auto launcherInjector = ProjectLauncherInjector::CreateInjector();

    m_Logger->LogDebug(Log("Creating project launcher...", "DwarfEngine"));
    auto launcher = launcherInjector.create<std::shared_ptr<ProjectLauncher>>();
    m_Logger->LogDebug(Log("Project launcher created.", "DwarfEngine"));
    m_Logger->LogDebug(Log("Running project launcher...", "DwarfEngine"));
    selectedProject = launcher->Run();
    m_Logger->LogDebug(
      Log("Project launcher finished running.", "DwarfEngine"));

    return selectedProject;
  }

  bool
  DwarfEngine::RunEditor(SavedProject selectedProject)
  {
    m_Logger->LogDebug(Log(
      "Opening project at: " + selectedProject.Path.string(), "DwarfEngine"));
    m_Logger->LogDebug(Log("Creating editor...", "DwarfEngine"));

    auto editorInjector = EditorInjector::CreateInjector(selectedProject);

    auto editor = editorInjector.create<std::shared_ptr<Dwarf::Editor>>();
    bool returnToLauncher = editor->Run();

    m_Logger->LogDebug(Log("Editor finished running.", "DwarfEngine"));
    m_Logger->LogDebug(
      Log("Return to launcher: " + std::to_string(returnToLauncher),
          "DwarfEngine"));

    return returnToLauncher;
  }
}
