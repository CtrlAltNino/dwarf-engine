#pragma once

#include "Core/Scene/IScene.h"

namespace Dwarf
{
  /**
   * @brief Class that manages the currently loaded scene in the editor
   *
   */
  class ILoadedScene
  {
  public:
    virtual ~ILoadedScene() = default;

    /**
     * @brief Gets the currently loaded scene
     *
     * @return Reference to the loaded scene instance
     */
    virtual auto
    GetScene() -> IScene& = 0;

    /**
     * @brief Sets the currently loaded scene
     *
     * @param scene The scene to set as the currently loaded scene
     */
    virtual void
    SetScene(std::unique_ptr<IScene> scene) = 0;

    /**
     * @brief Checks if a scene is set as the loaded scene
     *
     * @return true If a scene is present
     * @return false If no scene has been loaded
     */
    virtual auto
    HasLoadedScene() -> bool = 0;
  };
}