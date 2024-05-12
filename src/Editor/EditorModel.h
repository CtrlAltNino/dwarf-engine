#pragma once
#include "pch.h"
#include "Editor/IEditorModel.h"
#include "Core/Scene/Scene.h"

namespace Dwarf
{
  /// @brief Model of the Editor's MVC structure.
  class EditorModel : public IEditorModel
  {
  private:
    /// @brief The currently opened scene instance.
    std::shared_ptr<Scene> m_Scene;
    double                 m_DeltaTime;
    std::string            m_ProjectName;
    std::filesystem::path  m_ProjectPath;
    bool                   m_ReturnToLauncher = false;
    bool                   m_CloseSignal = false;
    EditorSelection        m_Selection;

  public:
    EditorModel(std::filesystem::path projectPath);

    /// @brief Returns the currently opened scene.
    /// @return A pointer to the scene.
    std::shared_ptr<Scene>
    GetScene() const override;

    void
    SetScene(std::shared_ptr<Scene> scene) override;

    std::string
    GetName() const override;

    std::filesystem::path
    GetProjectPath() const override;

    void
    SetDeltaTime(double deltaTime) override;

    double
    GetDeltaTime() const override;

    bool
    GetCloseSignal() const override;

    bool
    GetReturnToLauncher() const override;

    void
    CloseEditor(bool returnToLauncher) override;

    EditorSelection&
    GetSelection() override;
  };
}