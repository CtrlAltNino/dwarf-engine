#pragma once

#include "AssetBrowser/IAssetBrowserWindowFactory.h"
#include "Editor/Modules/AssetBrowser/IAssetBrowserWindowFactory.h"
#include "Editor/Modules/DebugInformation/IDebugWindowFactory.h"
#include "Editor/Modules/IGuiModuleFactory.h"
#include "Editor/Modules/Inspector/IInspectorWindowFactory.h"
#include "Editor/Modules/Performance/IPerformanceWindowFactory.h"
#include "Editor/Modules/SceneHierarchy/ISceneHierarchyWindowFactory.h"
#include "Editor/Modules/SceneViewer/ISceneViewerWindowFactory.h"

#include <boost/di.hpp>
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  class GuiModuleFactory : public IGuiModuleFactory
  {
  private:
    std::shared_ptr<IAssetBrowserWindowFactory>   mAssetBrowserWindowFactory;
    std::shared_ptr<IDebugWindowFactory>          mDebugWindowFactory;
    std::shared_ptr<IPerformanceWindowFactory>    mPerformanceWindowFactory;
    std::shared_ptr<ISceneHierarchyWindowFactory> mSceneHierarchyWindowFactory;
    std::shared_ptr<ISceneViewerWindowFactory>    mSceneViewerWindowFactory;
    std::shared_ptr<IInspectorWindowFactory>      mInspectorWindowFactory;

  public:
    GuiModuleFactory(
      std::shared_ptr<IAssetBrowserWindowFactory>   assetBrowserWindowFactory,
      std::shared_ptr<IDebugWindowFactory>          debugWindowFactory,
      std::shared_ptr<IPerformanceWindowFactory>    performanceWindowFactory,
      std::shared_ptr<ISceneHierarchyWindowFactory> sceneHierarchyWindowFactory,
      std::shared_ptr<ISceneViewerWindowFactory>    sceneViewerWindowFactory,
      std::shared_ptr<IInspectorWindowFactory>      inspectorWindowFactory);

    ~GuiModuleFactory() override = default;

    std::unique_ptr<IGuiModule>
    CreateGuiModule(MODULE_TYPE type) const override;

    std::unique_ptr<IGuiModule>
    CreateGuiModule(SerializedModule serializedModule) const override;
  };
} // namespace Dwarf