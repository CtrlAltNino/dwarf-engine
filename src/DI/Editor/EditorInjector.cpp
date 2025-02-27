#include "DI/Editor/EditorInjector.h"

#include "Core/Asset/AssetReference/AssetReferenceFactory.h"
#include "Core/Asset/AssetReference/IAssetReferenceFactory.h"
#include "Core/Asset/AssetReimporter/AssetReimporter.h"
#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Rendering/GpuInfo/GpuInfoFactory.h"
#include "Core/Rendering/GpuInfo/IGpuInfoFactory.h"
#include "Editor/Modules/Inspector/AssetInspector/MaterialAsset/IMaterialAssetInspector.h"
#include "Editor/Modules/Inspector/AssetInspector/MaterialAsset/MaterialAssetInspector.h"
#include "Editor/Modules/Inspector/AssetInspector/ModelAsset/IModelAssetInspector.h"
#include "Editor/Modules/Inspector/AssetInspector/ModelAsset/ModelAssetInspector.h"
#include "Editor/Modules/Inspector/AssetInspector/SceneAsset/ISceneAssetInspector.h"
#include "Editor/Modules/Inspector/AssetInspector/SceneAsset/SceneAssetInspector.h"
#include "Editor/Modules/Inspector/AssetInspector/TextureAsset/TextureAssetInspector.h"
#include "Logging/DwarfLogger.h"
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
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Asset/Shader/ShaderRecompiler.h"
#include "Core/Asset/Shader/ShaderSourceCollection/ShaderSourceCollectionFactory.h"
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
#include "Editor/Selection/IEditorSelection.h"
#include "Editor/Modules/Inspector/AssetInspector/IAssetInspector.h"
#include "Platform/OpenGL/IOpenGLStateTracker.h"
#include "Platform/OpenGL/OpenGLStateTracker.h"
#include "Project/ProjectSettings.h"
#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Core/Asset/Texture/ImageFileLoader.h"
#include "Project/ProjectSettingsIO.h"
#include "UI/IImGuiLayerFactory.h"
#include "UI/ImGuiLayerFactory.h"
#include "Core/Base.h"
#include "Core/Rendering/GraphicsContext/GraphicsContextFactory.h"
#include "Core/Rendering/GraphicsContext/IGraphicsContextFactory.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Core/Rendering/Texture/TextureFactory.h"
#include "Input/IInputManager.h"
#include "Input/InputManager.h"
#include "Editor/Editor.h"
#include "Editor/Stats/EditorStats.h"
#include "Project/ProjectTypes.h"
#include "Window/IWindow.h"
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Asset/Metadata/AssetMetadata.h"
#include "Core/Asset/Database/AssetDirectoryListener.h"
#include "Editor/Modules/Inspector/AssetInspector/AssetInspector.h"
#include "Editor/Modules/Inspector/EntityInspector/EntityInspector.h"
#include "Editor/Selection/EditorSelection.h"
#include "Core/Scene/Settings/SceneSettingsFactory.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Core/Rendering/VramTracker/VramTracker.h"
#include "Core/Asset/Texture/TextureWorker/TextureLoadingWorker.h"
#include <boost/di.hpp>
#include <boost/di/extension/scopes/shared.hpp>
#include "Core/Rendering/Shader/ShaderRegistry/ShaderRegistry.h"

#ifdef _WIN32
#include "Platform/Windows/WindowsWindow.h"
#elif __linux__
#include "Platform/Linux/LinuxWindow.h"
#endif

namespace Dwarf
{
  boost::di::injector<std::shared_ptr<Editor>>
  EditorInjector::CreateInjector(SavedProject selectedProject)
  {
    return boost::di::make_injector<boost::di::extension::shared_config>(
          boost::di::bind<LogName>.to(LogName("Editor")),
          boost::di::bind<IDwarfLogger>.to<DwarfLogger>().in(boost::di::extension::shared),
          boost::di::bind<GraphicsApi>.to(selectedProject.GraphicsApi),
          boost::di::bind<IGpuInfoFactory>.to<GpuInfoFactory>().in(boost::di::extension::shared),
          boost::di::bind<AssetDirectoryPath>.to(AssetDirectoryPath(selectedProject.Path / "Assets")),
          boost::di::bind<ProjectPath>.to(ProjectPath(selectedProject.Path)),
          boost::di::bind<ImGuiIniFilePath>.to(ImGuiIniFilePath(selectedProject.Path)),
          boost::di::bind<IFileHandler>.to<FileHandler>().in(boost::di::extension::shared),
          boost::di::bind<IProjectSettingsIO>.to<ProjectSettingsIO>().in(boost::di::extension::shared),
          boost::di::bind<IProjectSettings>.to<ProjectSettings>().in(boost::di::extension::shared),
          boost::di::bind<ICameraFactory>.to<CameraFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<IEditorStats>.to<EditorStats>().in(
          boost::di::extension::shared),
          boost::di::bind<IOpenGLStateTracker>.to<OpenGLStateTracker>().in(
          boost::di::extension::shared),
          boost::di::bind<IVramTracker>.to<VramTracker>().in(
          boost::di::extension::shared),
          boost::di::bind<IInputManager>.to<InputManager>().in(
          boost::di::extension::shared),
          boost::di::bind<IGraphicsContextFactory>.to<GraphicsContextFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<IAssetMetadata>.to<AssetMetadata>().in(
          boost::di::extension::shared),
          boost::di::bind<IMeshFactory>.to<MeshFactory>().in(
          boost::di::extension::shared),
          boost::di::bind<IModelImporter>.to<ModelImporter>().in(
          boost::di::extension::shared),
          boost::di::bind<IShaderRecompiler>.to<ShaderRecompiler>().in(
          boost::di::extension::shared),
          boost::di::bind<IImGuiLayerFactory>.to<ImGuiLayerFactory>().in(boost::di::extension::shared),
          boost::di::bind<WindowProps>.to(WindowProps(
          "Dwarf Engine", 1100, 600, selectedProject.GraphicsApi)),
#ifdef _WIN32
          boost::di::bind<IWindow>.to<WindowsWindow>().in(boost::di::extension::shared),
#elif __linux__
          boost::di::bind<IWindow>.to<LinuxWindow>().in(boost::di::extension::shared),
#endif
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
          boost::di::bind<ITextureLoadingWorker>.to<TextureLoadingWorker>().in(
          boost::di::extension::shared),
          boost::di::bind<IAssetReimporter>.to<AssetReimporter>().in(
          boost::di::extension::shared),
          boost::di::bind<ISceneIO>.to<SceneIO>().in(boost::di::extension::shared),
          boost::di::bind<IMaterialCreator>.to<MaterialCreator>().in(boost::di::extension::shared),
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
          boost::di::bind<IGuiModuleFactory>.to<GuiModuleFactory>().in(boost::di::extension::shared),
          boost::di::bind<IEditorView>.to<EditorView>().in(boost::di::extension::shared)
        );
  }
}