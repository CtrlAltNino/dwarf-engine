#pragma once
#include "Core/Scene/IScene.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Editor/IEditor.h"

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettings.h"
#include "Window/IWindow.h"
#include "Editor/EditorView/IEditorView.h"
#include "Editor/Stats/IEditorStats.h"
#include "Input/IInputManager.h"
#include <memory>

namespace Dwarf
{
  /// @brief The controller part of the editors MVC structure.
  class Editor : public IEditor
  {
  private:
    std::shared_ptr<IEditorView>      m_View;
    std::shared_ptr<IWindow>          m_Window;
    std::shared_ptr<IDwarfLogger>     m_Logger;
    std::shared_ptr<IEditorStats>     m_Stats;
    std::shared_ptr<IInputManager>    m_InputManager;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IScene>           m_Scene;
    std::shared_ptr<ISceneIO>         m_SceneIO;
    std::shared_ptr<IProjectSettings> m_ProjectSettings;

    bool m_ReturnToLauncher = false;
    bool m_CloseSignal = false;

  public:
    Editor(const std::shared_ptr<IDwarfLogger>&     logger,
           const std::shared_ptr<IEditorStats>&     stats,
           const std::shared_ptr<IInputManager>&    inputManager,
           const std::shared_ptr<IAssetDatabase>&   assetDatabase,
           const std::shared_ptr<IEditorView>&      view,
           const std::shared_ptr<IWindow>&          window,
           const std::shared_ptr<ISceneIO>&         sceneIO,
           const std::shared_ptr<IProjectSettings>& projectSettings);

    /// @brief Starts the render loop
    bool
    Run() override;

    void
    SetReturnToLauncher(bool returnToLauncher) override;

    bool
    GetReturnToLauncher() const override;

    void
    SetCloseSignal(bool closeSignal) override;

    bool
    GetCloseSignal() const override;

    void
    SetScene(std::shared_ptr<IScene> scene) override;

    std::shared_ptr<IScene>
    GetScene() const override;
  };
}