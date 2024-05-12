#pragma once
#include "pch.h"
#include "Core/Scene/Scene.h"
#include "Editor/EditorSelection.h"

namespace Dwarf
{

  /// @brief Model of the Editor's MVC structure.
  class IEditorModel
  {
  public:
    IEditorModel() = default;

    /// @brief Returns the currently opened scene.
    /// @return A pointer to the scene.
    virtual std::shared_ptr<Scene>
    GetScene() const = 0;

    virtual void
    SetScene(std::shared_ptr<Scene> scene) = 0;

    virtual std::string
    GetName() const = 0;

    virtual std::filesystem::path
    GetProjectPath() const = 0;

    virtual void
    SetDeltaTime(double deltaTime) = 0;

    virtual double
    GetDeltaTime() const = 0;

    virtual bool
    GetCloseSignal() const = 0;

    virtual bool
    GetReturnToLauncher() const = 0;

    virtual void
    CloseEditor(bool returnToLauncher) = 0;

    virtual EditorSelection&
    GetSelection() = 0;
  };
}