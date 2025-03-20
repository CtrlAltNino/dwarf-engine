#include "GuiModuleFactory.h"
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
    std::shared_ptr<IAssetBrowserWindowFactory>   assetBrowserWindowFactory,
    std::shared_ptr<IDebugWindowFactory>          debugWindowFactory,
    std::shared_ptr<IPerformanceWindowFactory>    performanceWindowFactory,
    std::shared_ptr<ISceneHierarchyWindowFactory> sceneHierarchyWindowFactory,
    std::shared_ptr<ISceneViewerWindowFactory>    sceneViewerWindowFactory,
    std::shared_ptr<IInspectorWindowFactory>      inspectorWindowFactory)
    : mAssetBrowserWindowFactory(assetBrowserWindowFactory)
    , mDebugWindowFactory(debugWindowFactory)
    , mPerformanceWindowFactory(performanceWindowFactory)
    , mSceneHierarchyWindowFactory(sceneHierarchyWindowFactory)
    , mSceneViewerWindowFactory(sceneViewerWindowFactory)
    , mInspectorWindowFactory(inspectorWindowFactory)
  {
  }

  std::unique_ptr<IGuiModule>
  GuiModuleFactory::CreateGuiModule(MODULE_TYPE type) const
  {
    switch (type)
    {
      using enum MODULE_TYPE;
      case ASSET_BROWSER: return mAssetBrowserWindowFactory->Create();
      case DEBUG: return mDebugWindowFactory->Create();
      case PERFORMANCE: return mPerformanceWindowFactory->Create();
      case SCENE_GRAPH: return mSceneHierarchyWindowFactory->Create();
      case SCENE_VIEWER: return mSceneViewerWindowFactory->Create();
      case INSPECTOR: return mInspectorWindowFactory->Create();
      default: return nullptr;
    }
  }

  std::unique_ptr<IGuiModule>
  GuiModuleFactory::CreateGuiModule(SerializedModule serializedModule) const
  {
    auto type = serializedModule.t["type"].get<MODULE_TYPE>();

    switch (type)
    {
      using enum MODULE_TYPE;
      case ASSET_BROWSER:
        return mAssetBrowserWindowFactory->Create(serializedModule);
      case DEBUG: return mDebugWindowFactory->Create(serializedModule);
      case PERFORMANCE:
        return mPerformanceWindowFactory->Create(serializedModule);
      case SCENE_GRAPH:
        return mSceneHierarchyWindowFactory->Create(serializedModule);
      case SCENE_VIEWER:
        return mSceneViewerWindowFactory->Create(serializedModule);
      case INSPECTOR: return mInspectorWindowFactory->Create(serializedModule);
      default: return nullptr;
    }
  }
} // namespace Dwarf