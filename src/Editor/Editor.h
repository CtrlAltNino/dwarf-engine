#pragma once
#include "Core/Asset/IAssetDatabase.h"
#include "Logging/EditorLogger.h"
#include "Window/Window.h"
#include "Editor/IEditorView.h"
#include "Editor/IEditor.h"
#include "Editor/IEditorStats.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  /// @brief The controller part of the editors MVC structure.
  class Editor : public IEditor
  {
  private:
    std::shared_ptr<IEditorView>    m_View;
    std::shared_ptr<Window>         m_Window;
    std::shared_ptr<EditorLogger>   m_Logger;
    std::shared_ptr<IEditorStats>   m_Stats;
    std::shared_ptr<IInputManager>  m_InputManager;
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;

  public:
    Editor(const std::shared_ptr<EditorLogger>&   logger,
           const std::shared_ptr<IEditorStats>&   stats,
           const std::shared_ptr<IInputManager>&  inputManager,
           const std::shared_ptr<IAssetDatabase>& assetDatabase);

    /// @brief Starts the render loop
    bool
    Run() override;
  };
}