#pragma once

#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Project/IProjectSettings.hpp"
#include "Window/IWindow.hpp"

namespace Dwarf
{
  struct CallbackContainer
  {
    UUID                  Id;
    std::function<void()> Callback;

    CallbackContainer(UUID id, std::function<void()> callback)
      : Id(std::move(id))
      , Callback(std::move(callback))
    {
    }
  };

  class LoadedScene : public ILoadedScene
  {
  private:
    GraphicsApi                       mGraphicsApi;
    std::shared_ptr<IDwarfLogger>     mLogger;
    std::unique_ptr<IScene>           mScene;
    std::shared_ptr<IWindow>          mWindow;
    std::shared_ptr<IProjectSettings> mProjectSettings;
    std::vector<CallbackContainer>    mSceneLoadCallback;
    std::vector<CallbackContainer>    mSceneUnloadCallback;
    std::vector<CallbackContainer>    mSceneChangeCallback;

  public:
    LoadedScene(GraphicsApi                       graphicsApi,
                std::shared_ptr<IDwarfLogger>     logger,
                std::shared_ptr<IWindow>          window,
                std::shared_ptr<IProjectSettings> projectSettings);
    ~LoadedScene() override;

    /**
     * @brief Gets the currently loaded scene
     *
     * @return Reference to the loaded scene instance
     */
    auto
    GetScene() -> IScene& override;

    /**
     * @brief Sets the currently loaded scene
     *
     * @param scene The scene to set as the currently loaded scene
     */
    void
    SetScene(std::unique_ptr<IScene> scene) override;

    /**
     * @brief Checks if a scene is set as the loaded scene
     *
     * @return true If a scene is present
     * @return false If no scene has been loaded
     */
    auto
    HasLoadedScene() -> bool override;

    /**
     * @brief Adds a callback that is called right after a scene has been loaded
     *
     * @param callback Callback function
     */
    auto
    AddSceneLoadCallback(std::function<void()> callback) -> UUID override;

    /**
     * @brief Removes a callback with the given Id
     *
     * @param id Uuid returned when emplacing the callback
     */
    void
    RemoveSceneLoadCallback(UUID id) override;

    /**
     * @brief Adds a callback that is called before the current scene is being
     * unloaded
     *
     * @param callback Callback function
     */
    auto
    AddSceneUnloadCallback(std::function<void()> callback) -> UUID override;

    /**
     * @brief Removes a callback with the given Id
     *
     * @param id Uuid returned when emplacing the callback
     */
    void
    RemoveSceneUnloadCallback(UUID id) override;

    /**
     * @brief Adds a callback that is called when something in the scene has
     * changed
     *
     * @param callback Callback function
     */
    auto
    AddSceneChangeCallback(std::function<void()> callback) -> UUID override;

    /**
     * @brief Removes a callback with the given Id
     *
     * @param id Uuid returned when emplacing the callback
     */
    void
    RemoveSceneChangeCallback(UUID id) override;

    /**
     * @brief Triggers the callbacks for when something in the scene changes
     *
     */
    void
    PropagateSceneChange() override;

    /**
     * @brief Updates the window title when a scene change occurs
     *
     */
    void
    UpdateWindowTitle() override;
  };
}