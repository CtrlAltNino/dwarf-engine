#pragma once

#include "Core/Scene/IScene.h"
namespace Dwarf
{
  /// @brief The controller part of the editors MVC structure.
  class IEditor
  {
  public:
    IEditor() = default;
    ~IEditor() = default;

    /// @brief Starts the render loop
    virtual bool
    Run() = 0;

    virtual void
    SetReturnToLauncher(bool returnToLauncher) = 0;

    virtual bool
    GetReturnToLauncher() const = 0;

    virtual void
    SetCloseSignal(bool closeSignal) = 0;

    virtual bool
    GetCloseSignal() const = 0;

    virtual void
    SetScene(std::shared_ptr<IScene> scene) = 0;

    virtual std::shared_ptr<IScene>
    GetScene() const = 0;
  };
}