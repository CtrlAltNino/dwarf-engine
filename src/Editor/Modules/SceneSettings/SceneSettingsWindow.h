#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
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
    std::shared_ptr<IRendererApi>   mRendererApi;

  public:
    SceneSettingsWindow(std::shared_ptr<IDwarfLogger>   logger,
                        std::shared_ptr<ILoadedScene>   loadedScene,
                        std::shared_ptr<IAssetDatabase> assetDatabase,
                        std::shared_ptr<IRendererApi>   rendererApi);

    SceneSettingsWindow(SerializedModule                serializedModule,
                        std::shared_ptr<IDwarfLogger>   logger,
                        std::shared_ptr<ILoadedScene>   loadedScene,
                        std::shared_ptr<IAssetDatabase> assetDatabase,
                        std::shared_ptr<IRendererApi>   rendererApi);

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