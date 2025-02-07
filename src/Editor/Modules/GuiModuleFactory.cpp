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
    : m_AssetBrowserWindowFactory(assetBrowserWindowFactory)
    , m_DebugWindowFactory(debugWindowFactory)
    , m_PerformanceWindowFactory(performanceWindowFactory)
    , m_SceneHierarchyWindowFactory(sceneHierarchyWindowFactory)
    , m_SceneViewerWindowFactory(sceneViewerWindowFactory)
    , m_InspectorWindowFactory(inspectorWindowFactory)
  {
  }

  std::unique_ptr<IGuiModule>
  GuiModuleFactory::CreateGuiModule(MODULE_TYPE type) const
  {
    switch (type)
    {
      using enum MODULE_TYPE;
      case ASSET_BROWSER: return m_AssetBrowserWindowFactory->Create();
      case DEBUG: return m_DebugWindowFactory->Create();
      case PERFORMANCE: return m_PerformanceWindowFactory->Create();
      case SCENE_GRAPH: return m_SceneHierarchyWindowFactory->Create();
      case SCENE_VIEWER: return m_SceneViewerWindowFactory->Create();
      case INSPECTOR: return m_InspectorWindowFactory->Create();
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
        return m_AssetBrowserWindowFactory->Create(serializedModule);
      case DEBUG: return m_DebugWindowFactory->Create(serializedModule);
      case PERFORMANCE:
        return m_PerformanceWindowFactory->Create(serializedModule);
      case SCENE_GRAPH:
        return m_SceneHierarchyWindowFactory->Create(serializedModule);
      case SCENE_VIEWER:
        return m_SceneViewerWindowFactory->Create(serializedModule);
      case INSPECTOR: return m_InspectorWindowFactory->Create(serializedModule);
      default: return nullptr;
    }
  }
} // namespace Dwarf