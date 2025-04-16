#include "pch.h"

#include "Editor/Modules/IGuiModule.h"
#include "GuiModuleFactory.h"

namespace Dwarf
{
  GuiModuleFactory::GuiModuleFactory(
    std::shared_ptr<IDwarfLogger>                 logger,
    std::shared_ptr<IAssetBrowserWindowFactory>   assetBrowserWindowFactory,
    std::shared_ptr<IDebugWindowFactory>          debugWindowFactory,
    std::shared_ptr<IPerformanceWindowFactory>    performanceWindowFactory,
    std::shared_ptr<ISceneHierarchyWindowFactory> sceneHierarchyWindowFactory,
    std::shared_ptr<ISceneViewerWindowFactory>    sceneViewerWindowFactory,
    std::shared_ptr<IInspectorWindowFactory>      inspectorWindowFactory,
    std::shared_ptr<ISceneSettingsWindowFactory>  sceneSettingsWindowFactory)
    : mLogger(std::move(logger))
    , mAssetBrowserWindowFactory(std::move(assetBrowserWindowFactory))
    , mDebugWindowFactory(std::move(debugWindowFactory))
    , mPerformanceWindowFactory(std::move(performanceWindowFactory))
    , mSceneHierarchyWindowFactory(std::move(sceneHierarchyWindowFactory))
    , mSceneViewerWindowFactory(std::move(sceneViewerWindowFactory))
    , mInspectorWindowFactory(std::move(inspectorWindowFactory))
    , mSceneSettingsWindowFactory(std::move(sceneSettingsWindowFactory))
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
      case SCENE_SETTINGS: return mSceneSettingsWindowFactory->Create();
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
      case SCENE_SETTINGS:
        return mSceneSettingsWindowFactory->Create(serializedModule);
      default: return nullptr;
    }
  }
} // namespace Dwarf