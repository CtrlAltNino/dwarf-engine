#include "pch.h"
#include "Launcher/ProjectLauncherData.h"
#include "Application/DwarfEngine.h"
#include "Editor/Modules/AssetBrowser/IAssetBrowserWindowFactory.h"
#include "Editor/Modules/DebugInformation/IDebugWindowFactory.h"
#include "Editor/Modules/Inspector/IInspectorWindowFactory.h"
#include "Editor/Modules/Performance/IPerformanceWindowFactory.h"
#include "Editor/Modules/SceneHierarchy/ISceneHierarchyWindowFactory.h"
#include "Editor/Modules/SceneViewer/ISceneViewerWindowFactory.h"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindowFactory.h"
#include "Editor/Modules/DebugInformation/DebugWindowFactory.h"
#include "Editor/Modules/Inspector/InspectorWindowFactory.h"
#include "Editor/Modules/Performance/PerformanceWindowFactory.h"
#include "Editor/Modules/SceneHierarchy/SceneHierarchyWindowFactory.h"
#include "Editor/Modules/SceneViewer/SceneViewerWindowFactory.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Database/AssetDatabase.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Core/Rendering/PreviewRenderer/ModelPreview/IModelPreview.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/MaterialPreview.h"
#include "Core/Rendering/PreviewRenderer/ModelPreview/ModelPreview.h"
#include "Editor/LoadedScene/LoadedScene.h"
#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Core/Asset/Creation/Material/MaterialCreator.h"
#include "Core/Asset/Database/IAssetDirectoryListener.h"
#include "Core/Asset/Model/ModelImporter.h"
#include "Core/Asset/Shader/IShaderSourceCollectionFactory.h"
#include "Core/Asset/Shader/ShaderRecompiler.h"
#include "Core/Asset/Shader/ShaderSourceCollectionFactory.h"
#include "Core/Rendering/Framebuffer/FramebufferFactory.h"
#include "Core/Rendering/Material/IO/MaterialIO.h"
#include "Core/Rendering/Material/MaterialFactory.h"
#include "Core/Rendering/Mesh/MeshFactory.h"
#include "Editor/Modules/IGuiModuleFactory.h"
#include "Editor/Modules/GuiModuleFactory.h"
#include "Core/Rendering/Pipelines/RenderingPipelineFactory.h"
#include "Core/Rendering/RendererApi/RendererApiFactory.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Core/Rendering/Shader/ShaderFactory.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/ShaderParameterCollectionFactory.h"
#include "Core/Scene/Camera/CameraFactory.h"
#include "Core/Scene/Camera/ICameraFactory.h"
#include "Core/Scene/IO/SceneIO.h"
#include "Core/Scene/ISceneFactory.h"
#include "Core/Scene/Properties/IScenePropertiesFactory.h"
#include "Core/Scene/Properties/ScenePropertiesFactory.h"
#include "Core/Scene/SceneFactory.h"
#include "Core/Scene/Settings/ISceneSettingsFactory.h"
#include "Editor/EditorView/EditorView.h"
#include "Editor/EditorView/IEditorView.h"
#include "Editor/IEditorSelection.h"
#include "Editor/Modules/Inspector/AssetInspector/IAssetInspector.h"
#include "Project/ProjectSettings.h"
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
#include "Launcher/IProjectLauncher.h"
#include "Launcher/ProjectLauncher.h"
#include "Launcher/View/IProjectLauncherView.h"
#include "Launcher/View/ProjectLauncherView.h"
#include "Editor/Editor.h"
#include "Editor/Stats/EditorStats.h"
#include "Project/ProjectTypes.h"
#include "Window/IWindow.h"
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Asset/Metadata/AssetMetadata.h"
#include "Core/Asset/Database/AssetDirectoryListener.h"
#include "Launcher/ProjectList/IProjectList.h"
#include "Launcher/ProjectList/ProjectList.h"
#include "Launcher/ProjectList/IO/IProjectListIO.h"
#include "Launcher/ProjectList/IO/ProjectListIO.h"
#include "Launcher/ProjectList/Sorter/IProjectListSorter.h"
#include "Launcher/ProjectList/Sorter/ProjectListSorter.h"
#include "Launcher/ProjectCreator/IProjectCreator.h"
#include "Launcher/ProjectCreator/ProjectCreator.h"
#include "Editor/Modules/Inspector/AssetInspector/AssetInspector.h"
#include "Editor/Modules/Inspector/EntityInspector/EntityInspector.h"
#include "Editor/EditorSelection.h"
#include "Core/Scene/Settings/SceneSettingsFactory.h"
#include <boost/di.hpp>

#ifdef _WIN32
#include "Platform/Windows/WindowsWindow.h"
#elif __linux__
#include "Platform/Linux/LinuxWindow.h"
#endif

namespace Dwarf
{
  DwarfEngine::DwarfEngine()
    : m_Logger(
        std::make_shared<DwarfLogger>(DwarfLogger(LogName("Application"))))
  {
  }

  void
  DwarfEngine::Run()
  {
    bool               returnToLauncher = true;
    ProjectInformation selectedProject = ProjectInformation();

    while (returnToLauncher)
    {
      selectedProject = RunLauncher();

      if (!selectedProject.path.t.empty())
      {
        returnToLauncher = RunEditor(selectedProject);
      }
      else
      {
        m_Logger->LogInfo(Log(
          "No project path provided. Exiting Dwarf Engine... ", "DwarfEngine"));
        returnToLauncher = false;
      }
    }
  }

  ProjectInformation
  DwarfEngine::RunLauncher()
  {
    ProjectInformation selectedProject = ProjectInformation();

    m_Logger->LogInfo(Log("Creating injector...", "DwarfEngine"));

    auto launcherInjector = boost::di::make_injector(
      boost::di::bind<LogName>.to(LogName("Launcher")),
      boost::di::bind<IDwarfLogger>.to<DwarfLogger>(),
      boost::di::bind<GraphicsApi>.to(GraphicsApi::OpenGL),
      boost::di::bind<IProjectLauncher>.to<ProjectLauncher>(),
      boost::di::bind<IImGuiLayerFactory>.to<ImGuiLayerFactory>(),
      boost::di::bind<IProjectLauncherData>.to<ProjectLauncherData>(),
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

    m_Logger->LogInfo(Log("Creating project launcher...", "DwarfEngine"));
    auto launcher = launcherInjector.create<ProjectLauncher>();
    m_Logger->LogInfo(Log("Project launcher created.", "DwarfEngine"));

    m_Logger->LogInfo(Log("Running project launcher...", "DwarfEngine"));
    selectedProject = launcher.Run();
    m_Logger->LogInfo(Log("Project launcher finished running.", "DwarfEngine"));

    return selectedProject;
  }

  bool
  DwarfEngine::RunEditor(ProjectInformation selectedProject)
  {
    m_Logger->LogInfo(Log(
      "Opening project at: " + selectedProject.path.t.string(), "DwarfEngine"));
    m_Logger->LogInfo(Log("Creating editor...", "DwarfEngine"));

    std::shared_ptr<AssetDirectoryPath> assetDirectoryPath =
      std::make_shared<AssetDirectoryPath>(selectedProject.path.t / "Assets");
    ProjectPath    projectPath = ProjectPath(selectedProject.path);
    SerializedView serializedView = SerializedView(std::nullopt);
    GraphicsApi    graphicsApi = selectedProject.graphicsApi;

    const auto editorInjector = boost::di::make_injector(
          boost::di::bind<LogName>.to(LogName("Editor")),
          boost::di::bind<IDwarfLogger>.to<DwarfLogger>().in(boost::di::singleton),
          boost::di::bind<GraphicsApi>.to(graphicsApi),
          boost::di::bind<AssetDirectoryPath>.to(assetDirectoryPath),
          boost::di::bind<ProjectPath>.to(projectPath),
          boost::di::bind<IProjectSettings>.to<ProjectSettings>().in(boost::di::singleton),
          boost::di::bind<SerializedView>.to(serializedView) ,
          boost::di::bind<ICameraFactory>.to<CameraFactory>().in(
          boost::di::singleton),
          boost::di::bind<IEditorStats>.to<EditorStats>().in(
          boost::di::singleton),
          boost::di::bind<IInputManager>.to<InputManager>().in(
          boost::di::singleton),
          boost::di::bind<IGraphicsContextFactory>.to<GraphicsContextFactory>().in(
          boost::di::singleton),
          boost::di::bind<IAssetMetadata>.to<AssetMetadata>().in(
          boost::di::singleton),
          boost::di::bind<IMeshFactory>.to<MeshFactory>().in(
          boost::di::singleton),
          boost::di::bind<IModelImporter>.to<ModelImporter>().in(
          boost::di::singleton),
          boost::di::bind<IShaderRecompiler>.to<ShaderRecompiler>().in(
          boost::di::singleton),
          boost::di::bind<ITextureFactory>.to<TextureFactory>().in(
          boost::di::singleton),
          boost::di::bind<IImageFileLoader>.to<ImageFileLoader>().in(
          boost::di::singleton),
          boost::di::bind<IImGuiLayerFactory>.to<ImGuiLayerFactory>().in(boost::di::singleton),
          boost::di::bind<ISceneSettingsFactory>.to<SceneSettingsFactory>().in(
          boost::di::singleton),
          boost::di::bind<IScenePropertiesFactory>.to<ScenePropertiesFactory>().in(
          boost::di::singleton),
          boost::di::bind<ISceneFactory>.to<SceneFactory>().in(
          boost::di::singleton),
          boost::di::bind<ILoadedScene>.to<LoadedScene>().in(
          boost::di::singleton),
          boost::di::bind<IEditorSelection>.to<EditorSelection>().in(
          boost::di::singleton),
          boost::di::bind<WindowProps>.to(WindowProps(
          "Dwarf Engine", 1100, 600, graphicsApi)),
#ifdef _WIN32
          boost::di::bind<IWindow>.to<WindowsWindow>().in(boost::di::singleton),
#elif __linux__
          boost::di::bind<IWindow>.to<LinuxWindow>().in(boost::di::singleton),
#endif
          boost::di::bind<IFramebufferFactory>.to<FramebufferFactory>().in(boost::di::singleton),
          boost::di::bind<IShaderParameterCollectionFactory>.to<ShaderParameterCollectionFactory>().in(boost::di::singleton),
          boost::di::bind<IShaderSourceCollectionFactory>.to<ShaderSourceCollectionFactory>().in(boost::di::singleton),
          boost::di::bind<IShaderFactory>.to<ShaderFactory>().in(boost::di::singleton),
          boost::di::bind<IMaterialFactory>.to<MaterialFactory>().in(boost::di::singleton),
          boost::di::bind<IMaterialIO>.to<MaterialIO>().in(boost::di::singleton),
          boost::di::bind<IAssetDirectoryListener>.to<AssetDirectoryListener>().in(boost::di::singleton),
          boost::di::bind<IAssetDatabase>.to<AssetDatabase>().in(
          boost::di::singleton),
          boost::di::bind<ISceneIO>.to<SceneIO>().in(boost::di::singleton),
          boost::di::bind<IMaterialCreator>.to<MaterialCreator>().in(boost::di::singleton),
          boost::di::bind<IRendererApiFactory>.to<RendererApiFactory>().in(boost::di::singleton),
          boost::di::bind<IRenderingPipelineFactory>.to<RenderingPipelineFactory>().in(boost::di::singleton),
          boost::di::bind<IMaterialPreview>.to<MaterialPreview>().in(boost::di::singleton),
          boost::di::bind<IModelPreview>.to<ModelPreview>().in(boost::di::singleton),
          boost::di::bind<IAssetInspector>.to<AssetInspector>().in(boost::di::singleton),
          boost::di::bind<IEntityInspector>.to<EntityInspector>().in(boost::di::singleton),
          boost::di::bind<IAssetBrowserWindowFactory>.to<AssetBrowserWindowFactory>().in(boost::di::singleton),
          boost::di::bind<IDebugWindowFactory>.to<DebugWindowFactory>().in(boost::di::singleton),
          boost::di::bind<IInspectorWindowFactory>.to<InspectorWindowFactory>().in(boost::di::singleton),
          boost::di::bind<IPerformanceWindowFactory>.to<PerformanceWindowFactory>().in(boost::di::singleton),
          boost::di::bind<ISceneHierarchyWindowFactory>.to<SceneHierarchyWindowFactory>().in(boost::di::singleton),
          boost::di::bind<ISceneViewerWindowFactory>.to<SceneViewerWindowFactory>().in(boost::di::singleton),
          boost::di::bind<IGuiModuleFactory>.to<GuiModuleFactory>().in(boost::di::singleton),
          boost::di::bind<IEditorView>.to<EditorView>().in(boost::di::singleton)
        );

    auto editor = editorInjector.create<std::shared_ptr<Dwarf::Editor>>();
    // returnToLauncher = !editor->Run();
    bool returnToLauncher = false;

    m_Logger->LogInfo(Log("Editor finished running.", "DwarfEngine"));
    m_Logger->LogInfo(
      Log("Return to launcheer: " + std::to_string(returnToLauncher),
          "DwarfEngine"));

    return returnToLauncher;
  }
}
