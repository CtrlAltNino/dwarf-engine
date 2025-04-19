#pragma once

#include "Core/Scene/IScene.hpp"

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

    /**
     * @brief Adds a callback that is called right after a scene has been loaded
     *
     * @param callback Callback function
     */
    virtual auto
    AddSceneLoadCallback(std::function<void()> callback) -> UUID = 0;

    /**
     * @brief Removes a callback with the given Id
     *
     * @param id Uuid returned when emplacing the callback
     */
    virtual void
    RemoveSceneLoadCallback(UUID id) = 0;

    /**
     * @brief Adds a callback that is called before the current scene is being
     * unloaded
     *
     * @param callback Callback function
     */
    virtual auto
    AddSceneUnloadCallback(std::function<void()> callback) -> UUID = 0;

    /**
     * @brief Removes a callback with the given Id
     *
     * @param id Uuid returned when emplacing the callback
     */
    virtual void
    RemoveSceneUnloadCallback(UUID id) = 0;

    /**
     * @brief Adds a callback that is called when something in the scene has
     * changed
     *
     * @param callback Callback function
     */
    virtual auto
    AddSceneChangeCallback(std::function<void()> callback) -> UUID = 0;

    /**
     * @brief Removes a callback with the given Id
     *
     * @param id Uuid returned when emplacing the callback
     */
    virtual void
    RemoveSceneChangeCallback(UUID id) = 0;

    /**
     * @brief Triggers the callbacks for when something in the scene changes
     *
     */
    virtual void
    PropagateSceneChange() = 0;

    /**
     * @brief Updates the window title when a scene change occurs
     *
     */
    virtual void
    UpdateWindowTitle() = 0;
  };
}