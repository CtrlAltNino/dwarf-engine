#pragma once

#include "Editor/LoadedScene/ILoadedScene.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettings.h"
#include "Window/IWindow.h"

namespace Dwarf
{
  class LoadedScene : public ILoadedScene
  {
  private:
    GraphicsApi                        mGraphicsApi;
    std::shared_ptr<IDwarfLogger>      mLogger;
    std::unique_ptr<IScene>            mScene;
    std::shared_ptr<IWindow>           mWindow;
    std::shared_ptr<IProjectSettings>  mProjectSettings;
    std::vector<std::function<void()>> mSceneLoadCallback;
    std::vector<std::function<void()>> mSceneUnloadCallback;
    std::vector<std::function<void()>> mSceneChangeCallback;

    /**
     * @brief Updates the window title when a scene change occurs
     *
     */
    void
    UpdateWindowTitle();

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
    void
    AddSceneLoadCallback(std::function<void()> callback) override;

    /**
     * @brief Adds a callback that is called before the current scene is being
     * unloaded
     *
     * @param callback Callback function
     */
    void
    AddSceneUnloadCallback(std::function<void()> callback) override;

    /**
     * @brief Adds a callback that is called when something in the scene has
     * changed
     *
     * @param callback Callback function
     */
    void
    AddSceneChangeCallback(std::function<void()> callback) override;

    /**
     * @brief Triggers the callbacks for when something in the scene changes
     *
     */
    void
    PropagateSceneChange() override;
  };
}