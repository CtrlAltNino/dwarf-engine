#pragma once

#include "AssetBrowser/IAssetBrowserWindowFactory.h"
#include "Editor/Modules/AssetBrowser/IAssetBrowserWindowFactory.h"
#include "Editor/Modules/DebugInformation/IDebugWindowFactory.h"
#include "Editor/Modules/IGuiModuleFactory.h"
#include "Editor/Modules/Inspector/IInspectorWindowFactory.h"
#include "Editor/Modules/Performance/IPerformanceWindowFactory.h"
#include "Editor/Modules/SceneHierarchy/ISceneHierarchyWindowFactory.h"
#include "Editor/Modules/SceneViewer/ISceneViewerWindowFactory.h"
#include "Logging/IDwarfLogger.h"
#include <boost/di.hpp>
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  class GuiModuleFactory : public IGuiModuleFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>                 mLogger;
    std::shared_ptr<IAssetBrowserWindowFactory>   mAssetBrowserWindowFactory;
    std::shared_ptr<IDebugWindowFactory>          mDebugWindowFactory;
    std::shared_ptr<IPerformanceWindowFactory>    mPerformanceWindowFactory;
    std::shared_ptr<ISceneHierarchyWindowFactory> mSceneHierarchyWindowFactory;
    std::shared_ptr<ISceneViewerWindowFactory>    mSceneViewerWindowFactory;
    std::shared_ptr<IInspectorWindowFactory>      mInspectorWindowFactory;

  public:
    GuiModuleFactory(
      std::shared_ptr<IDwarfLogger>                 logger,
      std::shared_ptr<IAssetBrowserWindowFactory>   assetBrowserWindowFactory,
      std::shared_ptr<IDebugWindowFactory>          debugWindowFactory,
      std::shared_ptr<IPerformanceWindowFactory>    performanceWindowFactory,
      std::shared_ptr<ISceneHierarchyWindowFactory> sceneHierarchyWindowFactory,
      std::shared_ptr<ISceneViewerWindowFactory>    sceneViewerWindowFactory,
      std::shared_ptr<IInspectorWindowFactory>      inspectorWindowFactory);

    ~GuiModuleFactory() override;

    /**
     * @brief Creates a gui module based on the specified type
     *
     * @param type Gui module type
     * @return Unique pointer to the created gui module
     */
    [[nodiscard]] auto
    CreateGuiModule(MODULE_TYPE type) const
      -> std::unique_ptr<IGuiModule> override;

    /**
     * @brief Creates a gui module based on a serialized structures
     *
     * @param serializedModule Serialized module json
     * @return Unique pointer to the created gui module
     */
    [[nodiscard]] auto
    CreateGuiModule(SerializedModule serializedModule) const
      -> std::unique_ptr<IGuiModule> override;
  };
} // namespace Dwarf