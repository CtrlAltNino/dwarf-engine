#pragma once

#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "ILoadedSceneObserver.h"
#include "Logging/IDwarfLogger.hpp"
#include "Project/IProjectSettings.hpp"
#include "Window/IWindow.hpp"

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
    std::vector<ILoadedSceneObserver*> mObservers;
    bool                               mHasLoadedScene = false;

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

    void
    RegisterLoadedSceneObserver(ILoadedSceneObserver* observer) override;

    void
    UnregisterLoadedSceneObserver(ILoadedSceneObserver* observer) override;

    /**
     * @brief Updates the window title when a scene change occurs
     *
     */
    void
    UpdateWindowTitle() override;
  };
}