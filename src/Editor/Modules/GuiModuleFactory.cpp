#include "GuiModuleFactory.h"
#include "Core/Asset/AssetTypes.h"
#include "Editor/IEditor.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include "Editor/Modules/DebugInformation/DebugWindow.h"
#include "Editor/Modules/IGuiModule.h"
#include "Editor/Modules/Inspector/AssetInspector/IAssetInspector.h"
#include "Editor/Modules/Inspector/InspectorWindow.h"
#include "Editor/Modules/Performance/PerformanceWindow.h"
#include "Editor/Modules/SceneHierarchy/SceneHierarchyWindow.h"
#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"
#include "Editor/Stats/IEditorStats.h"
#include <boost/di.hpp>
#include <memory>
#include <optional>

namespace Dwarf
{
  GuiModuleFactory::GuiModuleFactory(
    // AssetDirectoryPath assetDirectoryPath,
    // std::shared_ptr<ILoadedScene>        loadedScene,
    // std::shared_ptr<IEditorSelection> editorSelection,
    // std::shared_ptr<IAssetMetadata>      assetMetadata,
    // std::shared_ptr<IEditorStats>        editorStats,
    // std::shared_ptr<ITextureFactory>     textureFactory,
    // std::shared_ptr<IFramebufferFactory> framebufferFactory,
    // std::shared_ptr<ICameraFactory>            cameraFactory,
    // std::shared_ptr<IInputManager>             inputManager,
    // std::shared_ptr<IMaterialFactory>          materialFactory,
    // std::shared_ptr<IMaterialIO>               materialIO,
    // std::shared_ptr<IAssetDatabase>            assetDatabase,
    // std::shared_ptr<IRenderingPipelineFactory> renderingPipelineFactory,
    // std::shared_ptr<IMaterialCreator>          materialCreator,
    std::shared_ptr<IAssetInspector> assetInspector
    // std::shared_ptr<IEntityInspector> entityInspector
    )
    : m_CreateInjector(
        /*[&assetDirectoryPath,
         &textureFactory,
         &assetDatabase,
         &inputManager,
         &editorSelection,
         &materialIO,
         &materialFactory,
         &assetMetadata,
         &materialCreator,
         &assetInspector,
         &entityInspector,
         &editorStats,
         &loadedScene,
         &renderingPipelineFactory,
         &framebufferFactory,
         &cameraFactory]()
        {
          return boost::di::make_injector(
            boost::di::bind<AssetDirectoryPath>.to(assetDirectoryPath),
            boost::di::bind<ITextureFactory>.to(textureFactory),
            boost::di::bind<IAssetDatabase>.to(assetDatabase),
            boost::di::bind<IInputManager>.to(inputManager),
            boost::di::bind<IEditorSelection>.to(editorSelection),
            boost::di::bind<IMaterialIO>.to(materialIO),
            boost::di::bind<IMaterialFactory>.to(materialFactory),
            boost::di::bind<IAssetMetadata>.to(assetMetadata),
            boost::di::bind<IMaterialCreator>.to(materialCreator),
            boost::di::bind<IAssetInspector>.to(assetInspector),
            boost::di::bind<IEntityInspector>.to(entityInspector),
            boost::di::bind<IEditorStats>.to(editorStats),
            boost::di::bind<ILoadedScene>.to(loadedScene),
            boost::di::bind<IRenderingPipelineFactory>.to(
              renderingPipelineFactory),
            boost::di::bind<IFramebufferFactory>.to(framebufferFactory),
            boost::di::bind<ICameraFactory>.to(cameraFactory));
        }*/
        nullptr)
  {
  }

  std::shared_ptr<IGuiModule>
  GuiModuleFactory::CreateGuiModule(MODULE_TYPE type)
  {
    auto injector = boost::di::make_injector(
      m_CreateInjector(),
      boost::di::bind<SerializedModule>.to(SerializedModule(std::nullopt)));

    switch (type)
    {
      case MODULE_TYPE::ASSET_BROWSER:
        return std::make_shared<AssetBrowserWindow>(
          injector.create<AssetBrowserWindow>());
      case MODULE_TYPE::DEBUG:
        return std::make_shared<DebugWindow>(injector.create<DebugWindow>());
      case MODULE_TYPE::PERFORMANCE:
        return std::make_shared<PerformanceWindow>(
          injector.create<PerformanceWindow>());
      case MODULE_TYPE::SCENE_GRAPH:
        return std::make_shared<SceneHierarchyWindow>(
          injector.create<SceneHierarchyWindow>());
      case MODULE_TYPE::SCENE_VIEWER:
        return std::make_shared<SceneViewerWindow>(
          injector.create<SceneViewerWindow>());
      case MODULE_TYPE::INSPECTOR:
        return std::make_shared<InspectorWindow>(
          injector.create<InspectorWindow>());
      default: return nullptr;
    }
  }

  std::shared_ptr<IGuiModule>
  GuiModuleFactory::CreateGuiModule(nlohmann::json serializedModule)
  {
    auto type = serializedModule["type"].get<MODULE_TYPE>();
    auto injector =
      boost::di::make_injector(m_CreateInjector(),
                               boost::di::bind<SerializedModule>().to(
                                 SerializedModule(serializedModule)));

    switch (type)
    {
      case MODULE_TYPE::ASSET_BROWSER:
        return std::make_shared<AssetBrowserWindow>(
          injector.create<AssetBrowserWindow>());
      case MODULE_TYPE::DEBUG:
        return std::make_shared<DebugWindow>(injector.create<DebugWindow>());
      case MODULE_TYPE::PERFORMANCE:
        return std::make_shared<PerformanceWindow>(
          injector.create<PerformanceWindow>());
      case MODULE_TYPE::SCENE_GRAPH:
        return std::make_shared<SceneHierarchyWindow>(
          injector.create<SceneHierarchyWindow>());
      case MODULE_TYPE::SCENE_VIEWER:
        return std::make_shared<SceneViewerWindow>(
          injector.create<SceneViewerWindow>());
      case MODULE_TYPE::INSPECTOR:
        return std::make_shared<InspectorWindow>(
          injector.create<InspectorWindow>());
      default: return nullptr;
    }
  }
} // namespace Dwarf