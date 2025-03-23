#include "GuiModuleFactory.h"
#include "Editor/Modules/IGuiModule.h"
#include <boost/di.hpp>
#include <memory>
#include <utility>

namespace Dwarf
{
  GuiModuleFactory::GuiModuleFactory(
    std::shared_ptr<IDwarfLogger>                 logger,
    std::shared_ptr<IAssetBrowserWindowFactory>   assetBrowserWindowFactory,
    std::shared_ptr<IDebugWindowFactory>          debugWindowFactory,
    std::shared_ptr<IPerformanceWindowFactory>    performanceWindowFactory,
    std::shared_ptr<ISceneHierarchyWindowFactory> sceneHierarchyWindowFactory,
    std::shared_ptr<ISceneViewerWindowFactory>    sceneViewerWindowFactory,
    std::shared_ptr<IInspectorWindowFactory>      inspectorWindowFactory)
    : mLogger(std::move(logger))
    , mAssetBrowserWindowFactory(std::move(assetBrowserWindowFactory))
    , mDebugWindowFactory(std::move(debugWindowFactory))
    , mPerformanceWindowFactory(std::move(performanceWindowFactory))
    , mSceneHierarchyWindowFactory(std::move(sceneHierarchyWindowFactory))
    , mSceneViewerWindowFactory(std::move(sceneViewerWindowFactory))
    , mInspectorWindowFactory(std::move(inspectorWindowFactory))
  {
    mLogger->LogDebug(Log("GuiModuleFactory created", "GuiModuleFactory"));
  }

  GuiModuleFactory::~GuiModuleFactory()
  {
    mLogger->LogDebug(Log("GuiModuleFactory destroyed", "GuiModuleFactory"));
  }

  auto
  GuiModuleFactory::CreateGuiModule(MODULE_TYPE type) const
    -> std::unique_ptr<IGuiModule>
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

  auto
  GuiModuleFactory::CreateGuiModule(SerializedModule serializedModule) const
    -> std::unique_ptr<IGuiModule>
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