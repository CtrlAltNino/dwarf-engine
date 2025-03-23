#pragma once

#include "Editor/LoadedScene/ILoadedScene.h"
#include "Logging/IDwarfLogger.h"
#include "Project/IProjectSettings.h"

namespace Dwarf
{
  class LoadedScene : public ILoadedScene
  {
  private:
    std::shared_ptr<IDwarfLogger>     mLogger;
    std::unique_ptr<IScene>           mScene;
    std::shared_ptr<IProjectSettings> mProjectSettings;

  public:
    LoadedScene(std::shared_ptr<IDwarfLogger>     logger,
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
  };
}