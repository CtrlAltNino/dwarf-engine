#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Modules/IGuiModule.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class SceneSettingsWindow : public IGuiModule
  {
  private:
    std::shared_ptr<IDwarfLogger>   mLogger;
    std::shared_ptr<ILoadedScene>   mLoadedScene;
    std::shared_ptr<IAssetDatabase> mAssetDatabase;

  public:
    SceneSettingsWindow(std::shared_ptr<IDwarfLogger>   logger,
                        std::shared_ptr<ILoadedScene>   loadedScene,
                        std::shared_ptr<IAssetDatabase> assetDatabase);

    SceneSettingsWindow(SerializedModule                serializedModule,
                        std::shared_ptr<IDwarfLogger>   logger,
                        std::shared_ptr<ILoadedScene>   loadedScene,
                        std::shared_ptr<IAssetDatabase> assetDatabase);

    ~SceneSettingsWindow() override = default;

    void
    OnUpdate() override;

    void
    OnImGuiRender() override;

    auto
    Serialize() -> nlohmann::json override;

    void
    Deserialize(nlohmann::json moduleData);
  };
}