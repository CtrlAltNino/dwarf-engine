#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Rendering/RendererApi/IRendererApi.hpp"
#include "Core/Scene/ISceneObserver.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "Editor/Modules/IGuiModule.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  struct SkyboxSettingsState
  {
    SkyboxSource SkyboxSource;

    // Color Skybox
    glm::vec3 SkyboxColor;
    float     ColorExposure;

    // Material Skybox
    std::optional<UUID> SkyboxMaterialAssetId;

    // Hdri Skybox
    std::optional<UUID>   HdriTextureAssetId;
    float                 HdriExposure;
    CubemapResolutionEnum CubemapResolution;
    float                 CubemapRotation;
  };

  struct AmbientSettingsState
  {
    AmbientSource Type = AmbientSource::Color;
    /// @brief Color of the ambient light.
    glm::vec3 Color = { 0.2F, 0.2F, 0.2F };
    /// @brief Intensity of the ambient light.
    float                    Intensity = 1.0F;
    std::array<glm::vec3, 3> Gradient = {
      { { 0.5F, 0.7F, 1.0F }, { 0.6F, 0.6F, 0.6F }, { 0.2F, 0.2F, 0.25F } }
    };
    bool                UseIBL = false;
    std::optional<UUID> Cubemap = std::nullopt;
  };

  struct FogSettingsState
  {
    FogType Type = FogType::Linear;
    /// @brief Color of the fog.
    glm::vec3 Color = { 0.3F, 0.3F, 0.3F };
    /// @brief Starting distance of the fog.
    float Start = 20.0F;
    /// @brief Ending distance of the fog (only for linear fog).
    float End = 50.0F;
    float Density = 3.0F;
  };

  struct ShadowSettingsState
  {
    bool                Enabled = true;
    uint8_t             CascadeCount = 4;
    float               Distance = 50.0F;
    float               SplitLambda = 0.0F;
    ShadowMapResolution Resolution = ShadowMapResolution::Medium;
    float               DepthBias = 0.001F;
    float               SlopeScaledBias = 1.0F;
    uint8_t             PcfSamples = 1;
  };

  struct AntiAliasingSettingsState
  {
    AntiAliasingMethod Type = AntiAliasingMethod::None;
    uint8_t            MsaaSamples = 1;
  };

  struct ExposureSettingsState
  {
    ExposureType Type = ExposureType::Manual;
    float        Exposure = 1.5F;
  };

  struct BloomSettingsState
  {
    bool  Enabled = true;
    float Threshold = 0.7F;
    float Intensity = 1.0F;
    float Radius = 1.0F;
  };

  struct SceneSettingsWindowState
  {
    SkyboxSettingsState       SkyboxSettings;
    AmbientSettingsState      AmbientSettings;
    FogSettingsState          FogSettings;
    ShadowSettingsState       ShadowSettings;
    AntiAliasingSettingsState AntiAliasingSettings;
    TonemapType               TonemapType = TonemapType::Reinhard;
    ExposureSettingsState     ExposureSettings;
    BloomSettingsState        BloomSettings;
  };

  class SceneSettingsWindow
    : public IGuiModule
    , ILoadedSceneObserver
  {
  private:
    std::shared_ptr<IDwarfLogger>   mLogger;
    std::shared_ptr<ILoadedScene>   mLoadedScene;
    std::shared_ptr<IAssetDatabase> mAssetDatabase;
    std::shared_ptr<IRendererApi>   mRendererApi;
    SceneSettingsWindowState        mState;

    void
    RenderEnvironmentSettings();

    void
    RenderLightingSettings();

    void
    RenderRenderingSettings();

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

    ~SceneSettingsWindow() override;

    void
    OnUpdate() override;

    void
    OnImGuiRender() override;

    void
    OnSceneLoad() override;

    void
    OnSceneUnload() override;

    auto
    Serialize() -> nlohmann::json override;

    void
    Deserialize(nlohmann::json moduleData);
  };
}