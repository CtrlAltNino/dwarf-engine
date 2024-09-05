#include "GuiModuleFactory.h"
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
  GuiModuleFactory::GuiModuleFactory(CreateGuiModuleInjector createInjector)
    : m_CreateInjector(createInjector)
  {
  }

  std::shared_ptr<IGuiModule>
  GuiModuleFactory::CreateGuiModule(MODULE_TYPE type)
  {
    auto injector = m_CreateInjector();
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