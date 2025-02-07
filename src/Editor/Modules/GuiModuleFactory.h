#pragma once

#include "Editor/Modules/IGuiModuleFactory.h"
#include "AssetBrowser/IAssetBrowserWindowFactory.h"
#include "Editor/Modules/AssetBrowser/IAssetBrowserWindowFactory.h"
#include "Editor/Modules/DebugInformation/IDebugWindowFactory.h"
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
    std::shared_ptr<IAssetBrowserWindowFactory>   m_AssetBrowserWindowFactory;
    std::shared_ptr<IDebugWindowFactory>          m_DebugWindowFactory;
    std::shared_ptr<IPerformanceWindowFactory>    m_PerformanceWindowFactory;
    std::shared_ptr<ISceneHierarchyWindowFactory> m_SceneHierarchyWindowFactory;
    std::shared_ptr<ISceneViewerWindowFactory>    m_SceneViewerWindowFactory;
    std::shared_ptr<IInspectorWindowFactory>      m_InspectorWindowFactory;

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