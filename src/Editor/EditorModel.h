#pragma once
#include "pch.h"
#include "Editor/IEditorModel.h"
#include "Core/Scene/Scene.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief Model of the Editor's MVC structure.
  class EditorModel : public IEditorModel
  {
    BOOST_STRONG_TYPEDEF(std::filesystem::path, ProjectPath);

  private:
    /// @brief The currently opened scene instance.
    std::shared_ptr<Scene> m_Scene;
    std::string            m_ProjectName;
    double                 m_DeltaTime;
    ProjectPath            m_ProjectPath;
    EditorSelection        m_Selection;
    bool                   m_CloseSignal = false;
    bool                   m_ReturnToLauncher = false;

  public:
    EditorModel(ProjectPath projectPath);

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