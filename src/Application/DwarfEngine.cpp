#include "pch.h"
#include "Application/DwarfEngine.h"
#include "Launcher/ProjectLauncher.h"
#include "Editor/Editor.h"
#include "Logging/DwarfLogger.h"
#include "Project/ProjectTypes.h"
#include "DI/DwarfEditorDI.h"

namespace Dwarf
{
  void
  DwarfEngine::Run()
  {
    DwarfLogger logger = DwarfLogger();
    bool        shouldClose = false;
    while (!shouldClose)
    {
      std::filesystem::path projectPath = "";
      {
        logger.LogInfo(Log("Creating project launcher...", "DwarfEngine"));
        auto launcher = Dwarf::ProjectLauncher();
        logger.LogInfo(Log("Project launcher created.", "DwarfEngine"));

        logger.LogInfo(Log("Running project launcher...", "DwarfEngine"));
        launcher.Run(projectPath);
        logger.LogInfo(
          Log("Project launcher finished running.", "DwarfEngine"));
      }

      if (!projectPath.empty())
      {
        logger.LogInfo(
          Log("Opening project at: " + projectPath.string(), "DwarfEngine"));
        logger.LogInfo(Log("Creating editor...", "DwarfEngine"));
        // Create injector and bind the project path to the editor.
        const auto injector = Dwarf::DwarfEditorDI(projectPath);
        auto       editor = injector.Get().create<Dwarf::Editor>();
        shouldClose = !editor.Run();
        logger.LogInfo(Log("Editor finished running.", "DwarfEngine"));
        logger.LogInfo(
          Log("Should close: " + std::to_string(shouldClose), "DwarfEngine"));
      }
      else
      {
        logger.LogInfo(Log("No project path provided. Exiting Dwarf Engine...",
                           "DwarfEngine"));
        shouldClose = true;
      }
    }
  }
}
