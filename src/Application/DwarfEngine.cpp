#include "pch.h"
#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Core/Asset/Texture/ImageFileLoader.h"
#include "UI/IImGuiLayerFactory.h"
#include "UI/ImGuiLayerFactory.h"
#include "Core/Base.h"
#include "Core/Rendering/GraphicsContext/GraphicsContextFactory.h"
#include "Core/Rendering/GraphicsContext/IGraphicsContextFactory.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Core/Rendering/Texture/TextureFactory.h"
#include "Input/IInputManager.h"
#include "Input/InputManager.h"
#include "Application/DwarfEngine.h"
#include "Launcher/IProjectLauncher.h"
#include "Launcher/ProjectLauncher.h"
#include "Launcher/View/IProjectLauncherView.h"
#include "Launcher/View/ProjectLauncherView.h"
#include "Editor/Editor.h"
#include "Logging/DwarfLogger.h"
#include "Project/ProjectTypes.h"
#include "DI/DwarfEditorDI.h"
#include "Window/IWindow.h"
#include "Launcher/ProjectList/IProjectList.h"
#include "Launcher/ProjectList/ProjectList.h"
#include "Launcher/ProjectList/IO/IProjectListIO.h"
#include "Launcher/ProjectList/IO/ProjectListIO.h"
#include "Launcher/ProjectList/Sorter/IProjectListSorter.h"
#include "Launcher/ProjectList/Sorter/ProjectListSorter.h"
#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/ProjectCreator/ProjectCreator.h"

#ifdef _WIN32
#include "Platform/Windows/WindowsWindow.h"
#elif __linux__
#include "Platform/Linux/LinuxWindow.h"
#endif

namespace Dwarf
{
  void
  DwarfEngine::Run()
  {
    std::shared_ptr<DwarfLogger> logger = std::make_shared<DwarfLogger>();
    bool                         shouldClose = false;
    while (!shouldClose)
    {
      std::filesystem::path projectPath = "";
      {
        logger->LogInfo(Log("Creating injector...", "DwarfEngine"));
        auto injector = boost::di::make_injector(
          boost::di::bind<IDwarfLogger>.to(logger),
          boost::di::bind<GraphicsApi>.to(GraphicsApi::OpenGL),
          boost::di::bind<IProjectLauncher>.to<ProjectLauncher>(),
          boost::di::bind<IImGuiLayerFactory>.to<ImGuiLayerFactory>(),
#ifdef _WIN32
          boost::di::bind<IWindow>.to<WindowsWindow>(),
#elif __linux__
          boost::di::bind<IWindow>.to<LinuxWindow>(),
#endif
          boost::di::bind<IGraphicsContextFactory>.to<GraphicsContextFactory>(),
          boost::di::bind<IInputManager>.to<InputManager>(),
          boost::di::bind<IProjectLauncherView>.to<ProjectLauncherView>(),
          boost::di::bind<ITextureFactory>.to<TextureFactory>(),
          boost::di::bind<IImageFileLoader>.to<ImageFileLoader>(),
          boost::di::bind<IProjectList>.to<ProjectList>(),
          boost::di::bind<IProjectListIO>.to<ProjectListIO>(),
          boost::di::bind<IProjectListSorter>.to<ProjectListSorter>(),
          boost::di::bind<IProjectCreator>.to<ProjectCreator>(),
          boost::di::bind<WindowProps>.to(
            WindowProps("Dwarf Engine", 1100, 600, GraphicsApi::OpenGL)));

        logger->LogInfo(Log("Creating project launcher...", "DwarfEngine"));
        auto launcher = injector.create<std::shared_ptr<ProjectLauncher>>();
        logger->LogInfo(Log("Project launcher created.", "DwarfEngine"));

        logger->LogInfo(Log("Running project launcher...", "DwarfEngine"));
        ProjectPath selectedProjectPath = launcher->Run();
        logger->LogInfo(
          Log("Project launcher finished running.", "DwarfEngine"));
      }

      if (!projectPath.empty())
      {
        /*logger.LogInfo(
          Log("Opening project at: " + projectPath.string(), "DwarfEngine"));
        logger.LogInfo(Log("Creating editor...", "DwarfEngine"));
        // Create injector and bind the project path to the editor.
        const auto injector = Dwarf::DwarfEditorDI(projectPath);
        auto       editor = injector.Get().create<Dwarf::Editor>();
        shouldClose = !editor.Run();
        logger.LogInfo(Log("Editor finished running.", "DwarfEngine"));
        logger.LogInfo(
          Log("Should close: " + std::to_string(shouldClose), "DwarfEngine"));*/
      }
      else
      {
        logger->LogInfo(Log("No project path provided. Exiting Dwarf Engine...",
                            "DwarfEngine"));
        shouldClose = true;
      }
    }
  }
}
