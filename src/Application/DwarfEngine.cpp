#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Core/Rendering/PreviewRenderer/ModelPreview/IModelPreview.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/MaterialPreview.h"
#include "Core/Rendering/PreviewRenderer/ModelPreview/ModelPreview.h"
#include "Editor/LoadedScene/LoadedScene.h"
#include "pch.h"
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
#include "Application/DwarfEngine.h"
#include "Launcher/IProjectLauncher.h"
#include "Launcher/ProjectLauncher.h"
#include "Launcher/View/IProjectLauncherView.h"
#include "Launcher/View/ProjectLauncherView.h"
#include "Editor/Editor.h"
#include "Editor/Stats/EditorStats.h"
#include "Logging/DwarfLogger.h"
#include "Project/ProjectTypes.h"
#include "Window/IWindow.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Database/AssetDatabase.h"
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
#include <optional>

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
      ProjectInformation selectedProject = launcher->Run();
      logger->LogInfo(Log("Project launcher finished running.", "DwarfEngine"));

      if (!selectedProject.path.t.empty())
      {
        logger->LogInfo(
          Log("Opening project at: " + selectedProject.path.t.string(),
              "DwarfEngine"));
        logger->LogInfo(Log("Creating editor...", "DwarfEngine"));

        // Create injector and bind the project path to the editor.
        const auto injector = boost::di::make_injector(
          boost::di::bind<IDwarfLogger>.to(logger),
          boost::di::bind<GraphicsApi>.to(selectedProject.graphicsApi),
          boost::di::bind<AssetDirectoryPath>.to(
            AssetDirectoryPath(selectedProject.path.t / "Assets")),
          boost::di::bind<ICameraFactory>.to<CameraFactory>(),
          boost::di::bind<IEditorStats>.to<EditorStats>(),
          boost::di::bind<IInputManager>.to<InputManager>(),
          boost::di::bind<IGraphicsContextFactory>.to<GraphicsContextFactory>(),
          boost::di::bind<ProjectPath>.to(selectedProject.path),
          boost::di::bind<IAssetDirectoryListener>.to<AssetDirectoryListener>(),
          boost::di::bind<IAssetMetadata>.to<AssetMetadata>(),
          boost::di::bind<IMeshFactory>.to<MeshFactory>(),
          boost::di::bind<IModelImporter>.to<ModelImporter>(),
          boost::di::bind<IShaderRecompiler>.to<ShaderRecompiler>(),
          boost::di::bind<ITextureFactory>.to<TextureFactory>(),
          boost::di::bind<IImageFileLoader>.to<ImageFileLoader>(),
          boost::di::bind<IImGuiLayerFactory>.to<ImGuiLayerFactory>(),
          boost::di::bind<IProjectSettings>.to<ProjectSettings>(),
          boost::di::bind<ISceneSettingsFactory>.to<SceneSettingsFactory>(),
          boost::di::bind<IScenePropertiesFactory>.to<ScenePropertiesFactory>(),
          boost::di::bind<ISceneFactory>.to<SceneFactory>(),
          boost::di::bind<ILoadedScene>.to<LoadedScene>(),
          boost::di::bind<IEditorSelection>.to<EditorSelection>(),
          boost::di::bind<WindowProps>.to(WindowProps(
            "Dwarf Engine", 1100, 600, selectedProject.graphicsApi)),
#ifdef _WIN32
          boost::di::bind<IWindow>.to<WindowsWindow>(),
#elif __linux__
          boost::di::bind<IWindow>.to<LinuxWindow>(),
#endif
          boost::di::bind<IFramebufferFactory>.to<FramebufferFactory>(),
          boost::di::bind<IShaderParameterCollectionFactory>.to<ShaderParameterCollectionFactory>(),
          boost::di::bind<IShaderSourceCollectionFactory>.to<ShaderSourceCollectionFactory>(),
          boost::di::bind<IShaderFactory>.to<ShaderFactory>(),
          boost::di::bind<IMaterialFactory>.to<MaterialFactory>(),
          boost::di::bind<IMaterialIO>.to<MaterialIO>(),
          boost::di::bind<IAssetDatabase>.to<AssetDatabase>(),
          boost::di::bind<ISceneIO>.to<SceneIO>(),
          boost::di::bind<IMaterialCreator>.to<MaterialCreator>(),

          boost::di::bind<IRendererApiFactory>.to<RendererApiFactory>(),
          boost::di::bind<IRenderingPipelineFactory>.to<RenderingPipelineFactory>(),
          boost::di::bind<IMaterialPreview>.to<MaterialPreview>(),
          boost::di::bind<IModelPreview>.to<ModelPreview>(),
          boost::di::bind<IAssetInspector>.to<AssetInspector>(),
          boost::di::bind<IEntityInspector>.to<EntityInspector>(),
          boost::di::bind<SerializedView>.to(SerializedView(std::nullopt)),
          boost::di::bind<IGuiModuleFactory>.to<GuiModuleFactory>(),
          boost::di::bind<IEditorView>.to<EditorView>(),
          boost::di::bind<IEditor>.to<Editor>());

        auto editor = injector.create<Dwarf::AssetInspector>();
        //  shouldClose = !editor.Run();

        logger->LogInfo(Log("Editor finished running.", "DwarfEngine"));
        logger->LogInfo(
          Log("Should close: " + std::to_string(shouldClose), "DwarfEngine"));
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
