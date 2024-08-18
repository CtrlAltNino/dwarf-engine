#pragma once

#include "GuiModuleFactory.h"

#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include "Editor/Modules/DebugInformation/DebugWindow.h"
#include "Editor/Modules/Inspector/InspectorWindow.h"
#include "Editor/Modules/Performance/PerformanceWindow.h"
#include "Editor/Modules/SceneHierarchy/SceneHierarchyWindow.h"
#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"

namespace Dwarf
{
  std::shared_ptr<IGuiModule>
  GuiModuleFactory::CreateGuiModule(MODULE_TYPE type, int index)
  {
    switch (type)
    {
      case MODULE_TYPE::ASSET_BROWSER:
        return std::make_shared<AssetBrowserWindow>(type);
      case MODULE_TYPE::DEBUG: return std::make_shared<DebugWindow>(index);
      case MODULE_TYPE::PERFORMANCE:
        return std::make_shared<PerformanceWindow>(index);
      case MODULE_TYPE::SCENE_GRAPH:
        return std::make_shared<SceneHierarchyWindow>(
          m_Model, "Scene Hierarchy", type, m_GuiModuleIDCount++);
      case MODULE_TYPE::SCENE_VIEWER:
        return std::make_shared<SceneViewerModule>(
          m_Model, "Scene Viewer", type, m_GuiModuleIDCount++);
      case MODULE_TYPE::INSPECTOR:
        return std::make_shared<InspectorModule>(
          m_Model, "Inspector", type, m_GuiModuleIDCount++);
      default: return nullptr;
    }
  }

  std::shared_ptr<IGuiModule>
  GuiModuleFactory::CreateGuiModule(nlohmann::json serializedModule)
  {
    auto type = serializedModule["type"].get<MODULE_TYPE>();
    switch (type)
    {
      case MODULE_TYPE::PERFORMANCE:
        return std::make_shared<PerformanceModule>(m_Model, serializedModule);
      case MODULE_TYPE::SCENE_GRAPH:
        return std::make_shared<SceneGraphModule>(m_Model, serializedModule);
      case MODULE_TYPE::CONSOLE:
        return std::make_shared<ConsoleModule>(m_Model, serializedModule);
      case MODULE_TYPE::SCENE_VIEWER:
        return std::make_shared<SceneViewerModule>(m_Model, serializedModule);
      case MODULE_TYPE::ASSET_BROWSER:
        return std::make_shared<AssetBrowserModule>(m_Model, serializedModule);
      case MODULE_TYPE::INSPECTOR:
        return std::make_shared<InspectorModule>(m_Model, serializedModule);
      case MODULE_TYPE::DEBUG:
        return std::make_shared<DebugModule>(m_Model, serializedModule);
      default: return nullptr;
    }
  }
} // namespace Dwarf