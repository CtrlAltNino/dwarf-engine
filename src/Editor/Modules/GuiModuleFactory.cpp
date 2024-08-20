#pragma once

#include "GuiModuleFactory.h"

#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include "Editor/Modules/DebugInformation/DebugWindow.h"
#include "Editor/Modules/IGuiModule.h"
#include "Editor/Modules/Inspector/InspectorWindow.h"
#include "Editor/Modules/Performance/PerformanceWindow.h"
#include "Editor/Modules/SceneHierarchy/SceneHierarchyWindow.h"
#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"
#include <cstddef>
#include <memory>

namespace Dwarf
{
  GuiModuleFactory::GuiModuleFactory(boost::di::injector<IEditor>& editorDI)
    : m_EditorDI(editorDI)
  {
  }

  std::shared_ptr<IGuiModule>
  GuiModuleFactory::CreateGuiModule(MODULE_TYPE type)
  {
    switch (type)
    {
      case MODULE_TYPE::ASSET_BROWSER:
        return m_EditorDI.create<std::shared_ptr<AssetBrowserWindow>>();
      case MODULE_TYPE::DEBUG:
        return m_EditorDI.create<std::shared_ptr<DebugWindow>>();
      case MODULE_TYPE::PERFORMANCE:
        return m_EditorDI.create<std::shared_ptr<PerformanceWindow>>();
      case MODULE_TYPE::SCENE_GRAPH:
        return m_EditorDI.create<std::shared_ptr<SceneHierarchyWindow>>();
      case MODULE_TYPE::SCENE_VIEWER:
        return m_EditorDI.create<std::shared_ptr<SceneViewerWindow>>();
      case MODULE_TYPE::INSPECTOR:
        return m_EditorDI.create<std::shared_ptr<InspectorWindow>>();
      default: return nullptr;
    }
  }

  std::shared_ptr<IGuiModule>
  GuiModuleFactory::CreateGuiModule(nlohmann::json serializedModule)
  {
    return nullptr;
    // auto type = serializedModule["type"].get<MODULE_TYPE>();
    // switch (type)
    // {
    //   case MODULE_TYPE::PERFORMANCE:
    //     return std::make_shared<PerformanceModule>(m_Model,
    //     serializedModule);
    //   case MODULE_TYPE::SCENE_GRAPH:
    //     return std::make_shared<SceneGraphModule>(m_Model, serializedModule);
    //   case MODULE_TYPE::CONSOLE:
    //     return std::make_shared<ConsoleModule>(m_Model, serializedModule);
    //   case MODULE_TYPE::SCENE_VIEWER:
    //     return std::make_shared<SceneViewerModule>(m_Model,
    //     serializedModule);
    //   case MODULE_TYPE::ASSET_BROWSER:
    //     return std::make_shared<AssetBrowserModule>(m_Model,
    //     serializedModule);
    //   case MODULE_TYPE::INSPECTOR:
    //     return std::make_shared<InspectorModule>(m_Model, serializedModule);
    //   case MODULE_TYPE::DEBUG:
    //     return std::make_shared<DebugModule>(m_Model, serializedModule);
    //   default: return nullptr;
    // }
  }
} // namespace Dwarf