#include "pch.hpp"

#include "Core/Asset/AssetReference/AssetReferenceFactory.hpp"
#include "Core/Asset/AssetReference/IAssetReferenceFactory.hpp"
#include "Core/Asset/AssetReimporter/AssetReimporter.hpp"
#include "Core/Asset/AssetReimporter/IAssetReimporter.hpp"
#include "Core/Asset/Creation/Material/IMaterialCreator.hpp"
#include "Core/Asset/Creation/Material/MaterialCreator.hpp"
#include "Core/Asset/Creation/Shader/ShaderCreator.hpp"
#include "Core/Asset/Database/AssetDatabase.hpp"
#include "Core/Asset/Database/AssetDirectoryListener.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Asset/Database/IAssetDirectoryListener.hpp"
#include "Core/Asset/Metadata/AssetMetadata.hpp"
#include "Core/Asset/Metadata/IAssetMetadata.hpp"
#include "Core/Asset/Model/ModelImporter.hpp"
#include "Core/Asset/Shader/ShaderRecompiler.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/ShaderSourceCollectionFactory.hpp"
#include "Core/Asset/Texture/IImageFileLoader.hpp"
#include "Core/Asset/Texture/ImageFileLoader.hpp"
#include "Core/Asset/Texture/TextureWorker/TextureLoadingWorker.hpp"
#include "Core/Base.hpp"
#include "Core/Rendering/DrawCall/DrawCallFactory.hpp"
#include "Core/Rendering/DrawCall/DrawCallList/DrawCallListFactory.hpp"
#include "Core/Rendering/DrawCall/DrawCallWorker/DrawCallWorkerFactory.hpp"
#include "Core/Rendering/Framebuffer/FramebufferFactory.hpp"
#include "Core/Rendering/GraphicsContext/GraphicsContextFactory.hpp"
#include "Core/Rendering/GraphicsContext/IGraphicsContextFactory.hpp"
#include "Core/Rendering/Material/IO/MaterialIO.hpp"
#include "Core/Rendering/Material/MaterialFactory.hpp"
#include "Core/Rendering/Material/ShaderAssetSourceContainer/ShaderAssetSourceContainerFactory.hpp"
#include "Core/Rendering/Mesh/MeshFactory.hpp"
#include "Core/Rendering/MeshBuffer/MeshBufferFactory.hpp"
#include "Core/Rendering/MeshBuffer/MeshBufferRequestList/MeshBufferRequestList.hpp"
#include "Core/Rendering/PingPongBuffer/PingPongBufferFactory.hpp"
#include "Core/Rendering/Pipelines/RenderingPipelineFactory.hpp"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.hpp"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/MaterialPreview.hpp"
#include "Core/Rendering/PreviewRenderer/ModelPreview/IModelPreview.hpp"
#include "Core/Rendering/PreviewRenderer/ModelPreview/ModelPreview.hpp"
#include "Core/Rendering/RendererApi/RendererApiFactory.hpp"
#include "Core/Rendering/Shader/ShaderFactory.hpp"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.hpp"
#include "Core/Rendering/Shader/ShaderParameterCollection/ShaderParameterCollectionFactory.hpp"
#include "Core/Rendering/Shader/ShaderRegistry/ShaderRegistry.hpp"
#include "Core/Rendering/SkyboxRenderer/SkyboxRenderer.hpp"
#include "Core/Rendering/Texture/ITextureFactory.hpp"
#include "Core/Rendering/Texture/TextureFactory.hpp"
#include "Core/Rendering/VramTracker/IVramTracker.hpp"
#include "Core/Rendering/VramTracker/VramTracker.hpp"
#include "Core/Scene/Camera/CameraFactory.hpp"
#include "Core/Scene/Camera/ICameraFactory.hpp"
#include "Core/Scene/IO/SceneIO.hpp"
#include "Core/Scene/ISceneFactory.hpp"
#include "Core/Scene/Properties/IScenePropertiesFactory.hpp"
#include "Core/Scene/Properties/ScenePropertiesFactory.hpp"
#include "Core/Scene/SceneFactory.hpp"
#include "Core/Scene/Settings/ISceneSettingsFactory.hpp"
#include "Core/Scene/Settings/SceneSettingsFactory.hpp"
#include "DI/Editor/EditorInjector.hpp"
#include "Editor/Editor.hpp"
#include "Editor/EditorView/EditorView.hpp"
#include "Editor/EditorView/IEditorView.hpp"
#include "Editor/LoadedScene/LoadedScene.hpp"
#include "Editor/Modules/AssetBrowser/AssetBrowserListener/AssetBrowserListenerFactory.hpp"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindowFactory.hpp"
#include "Editor/Modules/AssetBrowser/IAssetBrowserWindowFactory.hpp"
#include "Editor/Modules/DebugInformation/DebugWindowFactory.hpp"
#include "Editor/Modules/DebugInformation/IDebugWindowFactory.hpp"
#include "Editor/Modules/GuiModuleFactory.hpp"
#include "Editor/Modules/IGuiModuleFactory.hpp"
#include "Editor/Modules/Inspector/AssetInspector/AssetInspector.hpp"
#include "Editor/Modules/Inspector/AssetInspector/IAssetInspector.hpp"
#include "Editor/Modules/Inspector/AssetInspector/MaterialAsset/IMaterialAssetInspector.hpp"
#include "Editor/Modules/Inspector/AssetInspector/MaterialAsset/MaterialAssetInspector.hpp"
#include "Editor/Modules/Inspector/AssetInspector/MaterialAsset/ShaderAssetSelector/ShaderAssetSelectorFactory.hpp"
#include "Editor/Modules/Inspector/AssetInspector/ModelAsset/IModelAssetInspector.hpp"
#include "Editor/Modules/Inspector/AssetInspector/ModelAsset/ModelAssetInspector.hpp"
#include "Editor/Modules/Inspector/AssetInspector/SceneAsset/ISceneAssetInspector.hpp"
#include "Editor/Modules/Inspector/AssetInspector/SceneAsset/SceneAssetInspector.hpp"
#include "Editor/Modules/Inspector/AssetInspector/TextureAsset/TextureAssetInspector.hpp"
#include "Editor/Modules/Inspector/EntityInspector/EntityInspector.hpp"
#include "Editor/Modules/Inspector/IInspectorWindowFactory.hpp"
#include "Editor/Modules/Inspector/InspectorWindowFactory.hpp"
#include "Editor/Modules/Performance/IPerformanceWindowFactory.hpp"
#include "Editor/Modules/Performance/PerformanceWindowFactory.hpp"
#include "Editor/Modules/SceneHierarchy/ISceneHierarchyWindowFactory.hpp"
#include "Editor/Modules/SceneHierarchy/SceneHierarchyWindowFactory.hpp"
#include "Editor/Modules/SceneSettings/SceneSettingsWindowFactory.hpp"
#include "Editor/Modules/SceneViewer/ISceneViewerWindowFactory.hpp"
#include "Editor/Modules/SceneViewer/SceneViewerWindowFactory.hpp"
#include "Editor/Selection/EditorSelection.hpp"
#include "Editor/Selection/IEditorSelection.hpp"
#include "Editor/Stats/EditorStats.hpp"
#include "Input/IInputManager.hpp"
#include "Input/InputManager.hpp"
#include "Logging/DwarfLogger.hpp"
#include "Platform/OpenGL/IOpenGLStateTracker.hpp"
#include "Platform/OpenGL/OpenGLStateTracker.hpp"
#include "Project/ProjectSettings.hpp"
#include "Project/ProjectSettingsIO.hpp"
#include "Project/ProjectTypes.hpp"
#include "UI/IImGuiLayerFactory.hpp"
#include "UI/ImGuiLayerFactory.hpp"
#include "Utilities/FileHandler/FileHandler.hpp"
#include "Window/IWindow.hpp"
#include "Window/SDL3Window.hpp"

namespace Dwarf
{
  auto
  EditorInjector::CreateInjector(SavedProject selectedProject)
    -> boost::di::injector<std::shared_ptr<Editor>>
  {
    return boost::di::make_injector<boost::di::extension::shared_config>(
          boost::di::bind<LogName>.to(LogName("Editor")),
          boost::di::bind<IDwarfLogger>.to<DwarfLogger>().in(boost::di::extension::shared),
          boost::di::bind<GraphicsApi>.to(selectedProject.Api),
          boost::di::bind<AssetDirectoryPath>.to(AssetDirectoryPath(selectedProject.Path / "Assets")),
          boost::di::bind<ProjectPath>.to(ProjectPath(selectedProject.Path)),
          boost::di::bind<ImGuiIniFilePath>.to(ImGuiIniFilePath(selectedProject.Path)),
          boost::di::bind<IFileHandler>.to<FileHandler>().in(boost::di::extension::shared),
          boost::di::bind<IProjectSettingsIO>.to<ProjectSettingsIO>().in(boost::di::extension::shared),
          boost::di::bind<IProjectSettings>.to<ProjectSettings>().in(boost::di::extension::shared),
          boost::di::bind<IAssetBrowserListenerFactory>.to<AssetBrowserListenerFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<ICameraFactory>.to<CameraFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<IEditorStats>.to<EditorStats>().in(
          boost::di::extension::shared),
          boost::di::bind<IOpenGLStateTracker>.to<OpenGLStateTracker>().in(
          boost::di::extension::shared),
          boost::di::bind<IDrawCallListFactory>.to<DrawCallListFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<IDrawCallFactory>.to<DrawCallFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<IVramTracker>.to<VramTracker>().in(
          boost::di::extension::shared),
          boost::di::bind<IInputManager>.to<InputManager>().in(
          boost::di::extension::shared),
          boost::di::bind<IGraphicsContextFactory>.to<GraphicsContextFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<IAssetMetadata>.to<AssetMetadata>().in(
          boost::di::extension::shared),
          boost::di::bind<IMeshBufferFactory>.to<MeshBufferFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<IMeshBufferRequestList>.to<MeshBufferRequestList>().in(
          boost::di::extension::shared),
          boost::di::bind<IMeshFactory>.to<MeshFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<IModelImporter>.to<ModelImporter>().in(
          boost::di::extension::shared),
          boost::di::bind<IShaderRecompiler>.to<ShaderRecompiler>().in(
          boost::di::extension::shared),
          boost::di::bind<IImGuiLayerFactory>.to<ImGuiLayerFactory>().in(boost::di::extension::shared),
          boost::di::bind<WindowProps>.to(WindowProps(
          "Dwarf Engine", 1100, 600, selectedProject.Api, true)),
          boost::di::bind<IWindow>.to<SDL3Window>().in(boost::di::extension::shared),
          boost::di::bind<ITextureFactory>.to<TextureFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<IImageFileLoader>.to<ImageFileLoader>().in(
          boost::di::extension::shared),
          boost::di::bind<ISceneSettingsFactory>.to<SceneSettingsFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<IScenePropertiesFactory>.to<ScenePropertiesFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<ISceneFactory>.to<SceneFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<ILoadedScene>.to<LoadedScene>().in(
          boost::di::extension::shared),
          boost::di::bind<IEditorSelection>.to<EditorSelection>().in(
          boost::di::extension::shared),
          boost::di::bind<IFramebufferFactory>.to<FramebufferFactory>().in(boost::di::extension::shared),
          boost::di::bind<IPingPongBufferFactory>.to<PingPongBufferFactory>().in(boost::di::extension::shared),
          boost::di::bind<IShaderParameterCollectionFactory>.to<ShaderParameterCollectionFactory>().in(boost::di::extension::shared),
          boost::di::bind<IShaderSourceCollectionFactory>.to<ShaderSourceCollectionFactory>().in(boost::di::extension::shared),
          boost::di::bind<IShaderFactory>.to<ShaderFactory>().in(boost::di::extension::shared),
          boost::di::bind<IShaderRegistry>.to<ShaderRegistry>().in(boost::di::extension::shared),
          boost::di::bind<IMaterialFactory>.to<MaterialFactory>().in(boost::di::extension::shared),
          boost::di::bind<IMaterialIO>.to<MaterialIO>().in(boost::di::extension::shared),
          boost::di::bind<IAssetDirectoryListener>.to<AssetDirectoryListener>().in(boost::di::extension::shared),
          boost::di::bind<IAssetReferenceFactory>.to<AssetReferenceFactory>().in(boost::di::extension::shared),
          boost::di::bind<IAssetDatabase>.to<AssetDatabase>().in(
          boost::di::extension::shared),
          boost::di::bind<ISkyboxRenderer>.to<SkyboxRenderer>().in(
            boost::di::extension::shared),
          boost::di::bind<IShaderAssetSourceContainerFactory>.to<ShaderAssetSourceContainerFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<IShaderAssetSelectorFactory>.to<ShaderAssetSelectorFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<ITextureLoadingWorker>.to<TextureLoadingWorker>().in(
          boost::di::extension::shared),
          boost::di::bind<IAssetReimporter>.to<AssetReimporter>().in(
          boost::di::extension::shared),
          boost::di::bind<IDrawCallWorkerFactory>.to<DrawCallWorkerFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<ISceneIO>.to<SceneIO>().in(boost::di::extension::shared),
          boost::di::bind<IMaterialCreator>.to<MaterialCreator>().in(boost::di::extension::shared),
          boost::di::bind<IShaderCreator>.to<ShaderCreator>().in(boost::di::extension::shared),
          boost::di::bind<IRendererApiFactory>.to<RendererApiFactory>().in(boost::di::extension::shared),
          boost::di::bind<IRenderingPipelineFactory>.to<RenderingPipelineFactory>().in(boost::di::extension::shared),
          boost::di::bind<IMaterialPreview>.to<MaterialPreview>().in(boost::di::extension::shared),
          boost::di::bind<IModelPreview>.to<ModelPreview>().in(boost::di::extension::shared),
          boost::di::bind<IMaterialAssetInspector>.to<MaterialAssetInspector>().in(boost::di::extension::shared),
          boost::di::bind<IModelAssetInspector>.to<ModelAssetInspector>().in(boost::di::extension::shared),
          boost::di::bind<ISceneAssetInspector>.to<SceneAssetInspector>().in(boost::di::extension::shared),
          boost::di::bind<ITextureAssetInspector>.to<TextureAssetInspector>().in(boost::di::extension::shared),
          boost::di::bind<IAssetInspector>.to<AssetInspector>().in(boost::di::extension::shared),
          boost::di::bind<IEntityInspector>.to<EntityInspector>().in(boost::di::extension::shared),
          boost::di::bind<IAssetBrowserWindowFactory>.to<AssetBrowserWindowFactory>().in(boost::di::extension::shared),
          boost::di::bind<IDebugWindowFactory>.to<DebugWindowFactory>().in(boost::di::extension::shared),
          boost::di::bind<IInspectorWindowFactory>.to<InspectorWindowFactory>().in(boost::di::extension::shared),
          boost::di::bind<IPerformanceWindowFactory>.to<PerformanceWindowFactory>().in(boost::di::extension::shared),
          boost::di::bind<ISceneHierarchyWindowFactory>.to<SceneHierarchyWindowFactory>().in(boost::di::extension::shared),
          boost::di::bind<ISceneViewerWindowFactory>.to<SceneViewerWindowFactory>().in(boost::di::extension::shared),
          boost::di::bind<ISceneSettingsWindowFactory>.to<SceneSettingsWindowFactory>().in(boost::di::extension::shared),
          boost::di::bind<IGuiModuleFactory>.to<GuiModuleFactory>().in(boost::di::extension::shared),
          boost::di::bind<IEditorView>.to<EditorView>().in(boost::di::extension::shared)
        );
  }
}