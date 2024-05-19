#include "Editor/EditorModel.h"
#include "Editor/EditorView.h"
#include "pch.h"
#include "Application/DwarfEngine.h"
#include "Launcher/ProjectLauncher.h"
#include "Editor/Editor.h"
#include "Logging/DefaultLogger.h"

namespace Dwarf
{
  void
  DwarfEngine::Run()
  {
    DefaultLogger logger = DefaultLogger();
    bool          shouldClose = false;
    while (!shouldClose)
    {
      std::filesystem::path projectPath = "";
      {
        logger.LogInfo("Creating project launcher...");
        auto launcher = Dwarf::ProjectLauncher();
        logger.LogInfo("Project launcher created.");

        logger.LogInfo("Running project launcher...");
        launcher.Run(projectPath);
        logger.LogInfo("Project launcher finished running.");
      }

      if (!projectPath.empty())
      {
        logger.LogInfo("Opening project at: " + projectPath.string());
        logger.LogInfo("Creating editor...");
        // Create injector and bind the project path to the editor.
        const auto injector = boost::di::make_injector(
          boost::di::bind<Dwarf::ProjectPath>.to(ProjectPath(projectPath)));
        auto editor = injector.create<Dwarf::Editor>();
        shouldClose = !editor.Run();
        logger.LogInfo("Editor finished running.");
        logger.LogInfo("Should close: " + std::to_string(shouldClose));
      }
      else
      {
        logger.LogInfo("No project path provided. Exiting Dwarf Engine...");
        shouldClose = true;
      }
    }
  }
}
