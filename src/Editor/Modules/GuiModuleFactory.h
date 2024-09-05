#pragma once

#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include "Editor/Modules/DebugInformation/DebugWindow.h"
#include "Editor/Modules/IGuiModuleFactory.h"
#include "Editor/Modules/Inspector/InspectorWindow.h"
#include "Editor/Modules/Performance/PerformanceWindow.h"
#include "Editor/Modules/SceneHierarchy/SceneHierarchyWindow.h"
#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"
#include <boost/di.hpp>
#include <functional>
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  using CreateGuiModuleInjector =
    std::function<boost::di::injector<AssetBrowserWindow,
                                      DebugWindow,
                                      PerformanceWindow,
                                      SceneHierarchyWindow,
                                      SceneViewerWindow,
                                      InspectorWindow>()>;

  class GuiModuleFactory : public IGuiModuleFactory
  {
  private:
    CreateGuiModuleInjector m_CreateInjector;

  public:
    explicit GuiModuleFactory(CreateGuiModuleInjector createInjector);

    ~GuiModuleFactory() override = default;
    std::shared_ptr<IGuiModule>
    CreateGuiModule(MODULE_TYPE type) override;

    std::shared_ptr<IGuiModule>
    CreateGuiModule(nlohmann::json serializedModule) override;
  };
} // namespace Dwarf