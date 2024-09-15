#pragma once
#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Asset/Shader/IShaderRecompiler.h"
#include "Core/Scene/IScene.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Core/Scene/ISceneFactory.h"
#include "Editor/IEditor.h"

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettings.h"
#include "Window/IWindow.h"
#include "Editor/EditorView/IEditorView.h"
#include "Editor/Stats/IEditorStats.h"
#include "Input/IInputManager.h"
#include <memory>

namespace Dwarf
{
  class Editor : public IEditor
  {
  private:
    std::shared_ptr<IEditorView>       m_View;
    std::shared_ptr<IWindow>           m_Window;
    std::shared_ptr<IDwarfLogger>      m_Logger;
    std::shared_ptr<IEditorStats>      m_EditorStats;
    std::shared_ptr<IInputManager>     m_InputManager;
    std::shared_ptr<ILoadedScene>      m_LoadedScene;
    std::shared_ptr<ISceneIO>          m_SceneIO;
    std::shared_ptr<ISceneFactory>     m_SceneFactory;
    std::shared_ptr<IProjectSettings>  m_ProjectSettings;
    std::shared_ptr<IAssetDatabase>    m_AssetDatabase;
    std::shared_ptr<IShaderRecompiler> m_ShaderRecompiler;
    std::shared_ptr<IAssetReimporter>  m_AssetReimporter;

  public:
    Editor(std::shared_ptr<IDwarfLogger>      logger,
           std::shared_ptr<IEditorStats>      stats,
           std::shared_ptr<IInputManager>     inputManager,
           std::shared_ptr<IProjectSettings>  projectSettings,
           std::shared_ptr<ILoadedScene>      loadedScene,
           std::shared_ptr<IWindow>           window,
           std::shared_ptr<ISceneIO>          sceneIO,
           std::shared_ptr<ISceneFactory>     sceneFactory,
           std::shared_ptr<IEditorView>       view,
           std::shared_ptr<IAssetDatabase>    assetDatabase,
           std::shared_ptr<IShaderRecompiler> shaderRecompiler,
           std::shared_ptr<IAssetReimporter>  assetReimporter);

    ~Editor() override = default;

    bool
    Run() override;
  };
}