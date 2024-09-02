#pragma once

#include "GuiModuleFactory.h"
#include "Editor/IEditor.h"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include "Editor/Modules/DebugInformation/DebugWindow.h"
#include "Editor/Modules/IGuiModule.h"
#include "Editor/Modules/Inspector/AssetInspector/IAssetInspector.h"
#include "Editor/Modules/Inspector/InspectorWindow.h"
#include "Editor/Modules/Performance/PerformanceWindow.h"
#include "Editor/Modules/SceneHierarchy/SceneHierarchyWindow.h"
#include "Editor/Modules/SceneViewer/SceneViewerWindow.h"
#include "Editor/Stats/IEditorStats.h"
#include <memory>
#include <optional>

namespace Dwarf
{
  GuiModuleFactory::GuiModuleFactory(
    AssetDirectoryPath                assetDirectoryPath,
    std::shared_ptr<ITextureFactory>  textureFactory,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IMaterialIO>      materialIO,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IAssetMetadata>   assetMetadata,
    std::shared_ptr<IMaterialCreator> materialCreator,
    std::shared_ptr<IEditorStats>     editorStats,
    std::shared_ptr<IEditor>          editor,
    std::shared_ptr<IAssetInspector>  assetInspector,
    std::shared_ptr<IEntityInspector> entityInspector,
    std::shared_ptr<ICameraFactory>   cameraFactory)
    : m_AssetDirectoryPath(assetDirectoryPath)
    , m_TextureFactory(textureFactory)
    , m_AssetDatabase(assetDatabase)
    , m_InputManager(inputManager)
    , m_EditorSelection(editorSelection)
    , m_MaterialIO(materialIO)
    , m_MaterialFactory(materialFactory)
    , m_AssetMetadata(assetMetadata)
    , m_MaterialCreator(materialCreator)
    , m_EditorStats(editorStats)
    , m_Editor(editor)
    , m_CameraFactory(cameraFactory)
    , m_AssetInspector(assetInspector)
    , m_EntityInspector(entityInspector)
  {
  }

  std::shared_ptr<IGuiModule>
  GuiModuleFactory::CreateGuiModule(MODULE_TYPE type)
  {
    switch (type)
    {
      case MODULE_TYPE::ASSET_BROWSER:
        return std::make_shared<AssetBrowserWindow>(
          AssetBrowserWindow(m_TextureFactory,
                             m_AssetDatabase,
                             m_InputManager,
                             m_EditorSelection,
                             m_MaterialIO,
                             m_MaterialFactory,
                             m_AssetMetadata,
                             m_MaterialCreator));
      case MODULE_TYPE::DEBUG:
        return std::make_shared<DebugWindow>(DebugWindow(m_AssetDatabase));
      case MODULE_TYPE::PERFORMANCE:
        return std::make_shared<PerformanceWindow>(m_EditorStats);
      case MODULE_TYPE::SCENE_GRAPH:
        return std::make_shared<SceneHierarchyWindow>(
          SceneHierarchyWindow(m_Editor, m_EditorSelection, m_InputManager));
      case MODULE_TYPE::SCENE_VIEWER:
        return std::make_shared<SceneViewerWindow>(
          SceneViewerWindow(m_CameraFactory,
                            m_FramebufferFactory,
                            m_EditorStats,
                            m_InputManager,
                            m_Editor,
                            m_EditorSelection,
                            m_RenderingPipelineFactory));
      case MODULE_TYPE::INSPECTOR:
        return std::make_shared<InspectorWindow>(
          InspectorWindow(std::nullopt,
                          m_Editor,
                          m_EditorSelection,
                          m_AssetDatabase,
                          m_AssetInspector,
                          m_EntityInspector));
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