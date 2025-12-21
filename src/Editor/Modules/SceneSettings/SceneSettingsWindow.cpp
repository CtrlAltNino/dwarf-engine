#include "pch.hpp"

#include "Core/Asset/Database/AssetComponents.hpp"
#include "Core/Rendering/AmbientTypes.hpp"
#include "Core/Rendering/AntiAliasingTypes.hpp"
#include "Core/Rendering/ShadowMappingTypes.hpp"
#include "Core/Rendering/SkyboxTypes.hpp"
#include "SceneSettingsWindow.hpp"
#include "UI/DwarfUI.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Dwarf
{
  SceneSettingsWindow::SceneSettingsWindow(
    std::shared_ptr<IDwarfLogger>   logger,
    std::shared_ptr<ILoadedScene>   loadedScene,
    std::shared_ptr<IAssetDatabase> assetDatabase,
    std::shared_ptr<IRendererApi>   rendererApi)
    : IGuiModule(ModuleLabel("Scene Settings"),
                 ModuleType(MODULE_TYPE::SCENE_SETTINGS),
                 ModuleID(std::make_shared<UUID>()))
    , mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
    , mAssetDatabase(std::move(assetDatabase))
    , mRendererApi(std::move(rendererApi))
  {
    mLoadedScene->RegisterLoadedSceneObserver(this);

    OnSceneLoad();
  }

  SceneSettingsWindow::SceneSettingsWindow(
    SerializedModule                serializedModule,
    std::shared_ptr<IDwarfLogger>   logger,
    std::shared_ptr<ILoadedScene>   loadedScene,
    std::shared_ptr<IAssetDatabase> assetDatabase,
    std::shared_ptr<IRendererApi>   rendererApi)
    : IGuiModule(ModuleLabel("Scene Settings"),
                 ModuleType(MODULE_TYPE::SCENE_SETTINGS),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
    , mAssetDatabase(std::move(assetDatabase))
    , mRendererApi(std::move(rendererApi))
  {
    mLoadedScene->RegisterLoadedSceneObserver(this);

    OnSceneLoad();
  }

  SceneSettingsWindow::~SceneSettingsWindow()
  {
    mLoadedScene->UnregisterLoadedSceneObserver(this);
  }

  void
  SceneSettingsWindow::OnUpdate()
  {
  }

  void
  SceneSettingsWindow::OnImGuiRender()
  {
    if (!mLoadedScene->HasLoadedScene())
    {
      return;
    }

    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoCollapse;

    if (!ImGui::Begin(GetIdentifier().c_str(), &mWindowOpened, windowFlags))
    {
      ImGui::End();
      return;
    }
    // ImGui::PopStyleVar(1);

    RenderEnvironmentSettings();

    RenderLightingSettings();

    RenderRenderingSettings();

    ImGui::End();
  }

  void
  SceneSettingsWindow::RenderEnvironmentSettings()
  {
    if (ImGui::CollapsingHeader("Environment & Atmosphere"))
    {
      // Skybox material
      ImGui::SeparatorText("Skybox");
      if (DwarfUI::ComboEnum("Source##Skybox",
                             mState.SkyboxSettings.SkyboxSource))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetSkyboxSettings()
          .SetType(mState.SkyboxSettings.SkyboxSource);
      }

      switch (mState.SkyboxSettings.SkyboxSource)
      {
        using enum SkyboxSource;
        case Color:
          {
            if (ImGui::ColorEdit3(
                  "Color##Skybox",
                  glm::value_ptr(mState.SkyboxSettings.SkyboxColor)))
            {
              mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetSkyboxSettings()
                .SetColor(mState.SkyboxSettings.SkyboxColor);
            }

            if (ImGui::DragFloat("Exposure##ColorExposure",
                                 &mState.SkyboxSettings.ColorExposure,
                                 0.25F,
                                 0.0F,
                                 150.0F,
                                 "%.2F"))
            {
              mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetSkyboxSettings()
                .SetExposure(mState.SkyboxSettings.ColorExposure);
            }
            break;
          }
        case Material:
          {
            if (DwarfUI::AssetInput<MaterialAsset>(
                  mAssetDatabase,
                  mState.SkyboxSettings.SkyboxMaterialAssetId,
                  "Material##Skybox"))
            {
              mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetSkyboxSettings()
                .SetSkyboxMaterial(mState.SkyboxSettings.SkyboxMaterialAssetId);
            }
          }
          break;
        case HDRI:
          {
            if (DwarfUI::AssetInput<TextureAsset>(
                  mAssetDatabase,
                  mState.SkyboxSettings.HdriTextureAssetId,
                  "HDRI##Skybox"))
            {
              mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetSkyboxSettings()
                .SetHdri(mState.SkyboxSettings.HdriTextureAssetId);
            }

            if (ImGui::DragFloat("Exposure##SkyboxExposure",
                                 &mState.SkyboxSettings.HdriExposure,
                                 0.25F,
                                 0.0F,
                                 150.0F,
                                 "%.2F"))
            {
              mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetSkyboxSettings()
                .SetExposure(mState.SkyboxSettings.HdriExposure);
            }

            if (DwarfUI::ComboEnum<CubemapResolutionEnum>(
                  "Resolution##CubemapResolution",
                  mState.SkyboxSettings.CubemapResolution))
            {
              mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetSkyboxSettings()
                .SetCubemapResolution(mState.SkyboxSettings.CubemapResolution);
            }

            if (ImGui::SliderFloat("Rotation##CubemapRotation",
                                   &mState.SkyboxSettings.CubemapRotation,
                                   0,
                                   360,
                                   "%.2F"))
            {
              mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetSkyboxSettings()
                .SetCubemapRotation(mState.SkyboxSettings.CubemapRotation);
            }
          }
          break;
      }

      // Ambient light color / intensity
      ImGui::SeparatorText("Ambient Lighting Settings");
      DwarfUI::ComboEnum<AmbientSource>("Source##Ambient",
                                        mState.AmbientSettings.Type);

      switch (mState.AmbientSettings.Type)
      {
        using enum AmbientSource;
        case None:
        case Skybox: break;
        case Color:
          if (ImGui::ColorEdit3("Color##Ambient",
                                glm::value_ptr(mState.AmbientSettings.Color)))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetAmbientSettings()
              .SetAmbientLightColor(mState.AmbientSettings.Color);
          }

          if (ImGui::DragFloat("Intensity##Ambient",
                               &mState.AmbientSettings.Intensity,
                               0.2F,
                               0.0F,
                               +FLT_MAX,
                               "%.3f"))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetAmbientSettings()
              .SetAmbientIntensity(mState.AmbientSettings.Intensity);
          }
          break;
        case Gradient:
          if (ImGui::ColorEdit3(
                "Sky##Ambient",
                glm::value_ptr(mState.AmbientSettings.Gradient[0])))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetAmbientSettings()
              .SetGradient(mState.AmbientSettings.Gradient);
          }
          if (ImGui::ColorEdit3(
                "Horizon##Ambient",
                glm::value_ptr(mState.AmbientSettings.Gradient[1])))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetAmbientSettings()
              .SetGradient(mState.AmbientSettings.Gradient);
          }
          if (ImGui::ColorEdit3(
                "Ground##Ambient",
                glm::value_ptr(mState.AmbientSettings.Gradient[2])))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetAmbientSettings()
              .SetGradient(mState.AmbientSettings.Gradient);
          }
          if (ImGui::DragFloat("Intensity##Ambient",
                               &mState.AmbientSettings.Intensity,
                               0.2F,
                               0.0F,
                               +FLT_MAX,
                               "%.3f"))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetAmbientSettings()
              .SetAmbientIntensity(mState.AmbientSettings.Intensity);
          }
          break;
        case HDRI:
          if (DwarfUI::AssetInput<TextureAsset>(mAssetDatabase,
                                                mState.AmbientSettings.Cubemap,
                                                "HDRI##Ambient"))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetAmbientSettings()
              .SetCubeMap(mState.AmbientSettings.Cubemap);
          }

          if (ImGui::Checkbox("Use for IBL##Ambient",
                              &mState.AmbientSettings.UseIBL))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetAmbientSettings()
              .SetUseIBL(mState.AmbientSettings.UseIBL);
          }
          break;
      }

      // Fog settings
      ImGui::SeparatorText("Fog Settings");
      if (DwarfUI::ComboEnum<FogType>("Fog Type##Fog", mState.FogSettings.Type))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetFogSettings()
          .SetFogType(mState.FogSettings.Type);
      }

      if (ImGui::ColorEdit3("Color##Fog",
                            glm::value_ptr(mState.FogSettings.Color)))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetFogSettings()
          .SetColor(mState.FogSettings.Color);
      }

      switch (mState.FogSettings.Type)
      {
        using enum FogType;
        case Linear:
          if (ImGui::DragFloatRange2("Distance##Fog",
                                     &mState.FogSettings.Start,
                                     &mState.FogSettings.End,
                                     0.25F,
                                     0.0F,
                                     +FLT_MAX,
                                     "Start: %.1f",
                                     "End: %.1f",
                                     ImGuiSliderFlags_AlwaysClamp))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetFogSettings()
              .SetStart(mState.FogSettings.Start);
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetFogSettings()
              .SetEnd(mState.FogSettings.End);
          }
          break;
        case Exponential:
          if (ImGui::DragFloat("Density##Fog",
                               &mState.FogSettings.Density,
                               0.005F,
                               0.0F,
                               +FLT_MAX,
                               "%.3f",
                               0))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetFogSettings()
              .SetDensity(mState.FogSettings.Density);
          }
          break;
      }
    }
  }

  void
  SceneSettingsWindow::RenderLightingSettings()
  {
    if (ImGui::CollapsingHeader("Lighting"))
    {
      // Global shadow settings
      ImGui::SeparatorText("Shadow Settings");

      // Enable/disable shadow mapping
      if (ImGui::Checkbox("Enable##ShadowMapping",
                          &mState.ShadowSettings.Enabled))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetShadowSettings()
          .SetEnabled(mState.ShadowSettings.Enabled);
      }

      // Shadow map resolution
      if (DwarfUI::ComboEnum<ShadowMapResolution>(
            "Shadow Map Resolution##ShadowMapping",
            mState.ShadowSettings.Resolution))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetShadowSettings()
          .SetResolution(mState.ShadowSettings.Resolution);
      }

      // Number of cascades (clamped between 1–4 typically)
      static uint8_t minCascades = 1U;
      static uint8_t maxCascades = 4U;
      if (ImGui::SliderScalar("Cascade Count##ShadowMapping",
                              ImGuiDataType_U8,
                              &mState.ShadowSettings.CascadeCount,
                              &minCascades,
                              &maxCascades))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetShadowSettings()
          .SetCascadeCount(mState.ShadowSettings.CascadeCount);
      }

      // Shadow distance (far plane for cascades)
      if (ImGui::DragFloat("Shadow Distance##ShadowMapping",
                           &mState.ShadowSettings.Distance,
                           10.0F,
                           10.0F,
                           +FLT_MAX,
                           "%.1f m"))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetShadowSettings()
          .SetDistance(mState.ShadowSettings.Distance);
      }

      // Split scheme factor (lambda between uniform and log split)
      if (ImGui::SliderFloat("Cascade Split Lambda##ShadowMapping",
                             &mState.ShadowSettings.SplitLambda,
                             0.0F,
                             1.0F))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetShadowSettings()
          .SetSplitLambda(mState.ShadowSettings.SplitLambda);
      }

      // Bias
      if (ImGui::SliderFloat("Depth Bias##ShadowMapping",
                             &mState.ShadowSettings.DepthBias,
                             0.0F,
                             0.01F,
                             "%.5f"))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetShadowSettings()
          .SetDepthBias(mState.ShadowSettings.DepthBias);
      }

      if (ImGui::SliderFloat("Slope Bias##ShadowMapping",
                             &mState.ShadowSettings.SlopeScaledBias,
                             0.0F,
                             5.0F,
                             "%.2f"))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetShadowSettings()
          .SetSlopeScaledBias(mState.ShadowSettings.SlopeScaledBias);
      }

      // Filtering
      static uint8_t minPfcSamples = 1;
      static uint8_t maxPfcSamples = 64;
      if (ImGui::SliderScalar("PCF Samples##ShadowMapping",
                              ImGuiDataType_U8,
                              &mState.ShadowSettings.PcfSamples,
                              &minPfcSamples,
                              &maxPfcSamples))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetShadowSettings()
          .SetPcfSamples(mState.ShadowSettings.PcfSamples);
      }

      // Visual debug toggle
      // ImGui::Checkbox("Visualize Cascades", &settings.visualizeCascades);
    }
  }

  void
  SceneSettingsWindow::RenderRenderingSettings()
  {
    if (ImGui::CollapsingHeader("Rendering & Post-Processing"))
    {
      ImGui::SeparatorText("Anti-Aliasing");
      // Anti-Aliasing Method
      if (DwarfUI::ComboEnum<AntiAliasingMethod>(
            "Anti-Aliasing Method##AntiAliasing",
            mState.AntiAliasingSettings.Type))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetAntiAliasingSettings()
          .SetAntiAliasingMethod(mState.AntiAliasingSettings.Type);
      }

      switch (mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetAntiAliasingSettings()
                .GetAntiAliasingMethod())
      {
        using enum AntiAliasingMethod;
        case None:
        case FXAA:
        case TAA: break;
        case MSAA:
          static uint8_t min = 1U;
          static uint8_t max = mRendererApi->GetMaxSamples();
          std::string    format = std::format("{}/{}", "%d", max);

          if (ImGui::SliderScalar("MSAA Samples##AntiAliasing",
                                  ImGuiDataType_U8,
                                  &mState.AntiAliasingSettings.MsaaSamples,
                                  &min,
                                  &max,
                                  format.c_str(),
                                  ImGuiSliderFlags_None))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetAntiAliasingSettings()
              .SetSamples(mState.AntiAliasingSettings.MsaaSamples);
          }
          break;
      }

      // Tonemapping
      ImGui::SeparatorText("Tonemapping");
      if (DwarfUI::ComboEnum<TonemapType>("Tonemapping##Tonemapping",
                                          mState.TonemapType))
      {
        mLoadedScene->GetScene().GetProperties().GetSettings().SetToneMapType(
          mState.TonemapType);
      }

      // Global exposure
      ImGui::SeparatorText("Global Exposure##Exposure");
      if (DwarfUI::ComboEnum<ExposureType>("Exposure Type##Exposure",
                                           mState.ExposureSettings.Type))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetExposureSettings()
          .SetExposureType(mState.ExposureSettings.Type);
      }

      switch (mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetExposureSettings()
                .GetExposureType())
      {
        using enum ExposureType;
        case Manual:
          if (ImGui::DragFloat("Exposure##Exposure",
                               &mState.ExposureSettings.Exposure,
                               0.0005F,
                               0.0F,
                               20.0F,
                               "%f"))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetExposureSettings()
              .SetExposure(mState.ExposureSettings.Exposure);
          }
          break;
        case Automatic:
          // TODO: Implement automatic exposure system
          break;
      }

      // Bloom Settings
      ImGui::SeparatorText("Bloom");

      // Toggle bloom enabled status
      if (ImGui::Checkbox("Enable##Bloom", &mState.BloomSettings.Enabled))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetBloomSettings()
          .SetEnabled(mState.BloomSettings.Enabled);
      }

      // Bloom Threshold
      if (ImGui::DragFloat("Threshold##Bloom",
                           &mState.BloomSettings.Threshold,
                           0.01F,
                           0.0F,
                           +FLT_MAX,
                           "%.2f"))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetBloomSettings()
          .SetThreshold(mState.BloomSettings.Threshold);
      }

      if (ImGui::DragFloat("Intensity##Bloom",
                           &mState.BloomSettings.Intensity,
                           0.01F,
                           0.0F,
                           +FLT_MAX,
                           "%.2f"))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetBloomSettings()
          .SetIntensity(mState.BloomSettings.Intensity);
      }
      if (ImGui::DragFloat("Radius##Bloom",
                           &mState.BloomSettings.Radius,
                           0.01F,
                           0.0F,
                           +FLT_MAX,
                           "%.2f"))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetBloomSettings()
          .SetRadius(mState.BloomSettings.Radius);
      }

      // Depth of Field Settings
      /*ImGui::SeparatorText("Depth of Field");

      // Color Grading Settings
      ImGui::SeparatorText("Color Grading");

      // Ambient Occlusion Settings
      ImGui::SeparatorText("Ambient Occlusion");

      // Screen Space Reflections Settings
      ImGui::SeparatorText("Screen Space Reflections");*/
    }
  }

  void
  SceneSettingsWindow::OnSceneLoad()
  {
    // Copy Scene Settings into state
    if (mLoadedScene->HasLoadedScene())
    {
      mState.SkyboxSettings.SkyboxSource = mLoadedScene->GetScene()
                                             .GetProperties()
                                             .GetSettings()
                                             .GetSkyboxSettings()
                                             .GetType();
      mState.SkyboxSettings.SkyboxColor = mLoadedScene->GetScene()
                                            .GetProperties()
                                            .GetSettings()
                                            .GetSkyboxSettings()
                                            .GetColor();
      mState.SkyboxSettings.ColorExposure = mLoadedScene->GetScene()
                                              .GetProperties()
                                              .GetSettings()
                                              .GetSkyboxSettings()
                                              .GetExposure();
      mState.SkyboxSettings.SkyboxMaterialAssetId = mLoadedScene->GetScene()
                                                      .GetProperties()
                                                      .GetSettings()
                                                      .GetSkyboxSettings()
                                                      .GetSkyboxMaterial();
      mState.SkyboxSettings.HdriTextureAssetId = mLoadedScene->GetScene()
                                                   .GetProperties()
                                                   .GetSettings()
                                                   .GetSkyboxSettings()
                                                   .GetHdri();
      mState.SkyboxSettings.HdriExposure = mLoadedScene->GetScene()
                                             .GetProperties()
                                             .GetSettings()
                                             .GetSkyboxSettings()
                                             .GetExposure();
      mState.SkyboxSettings.CubemapResolution = mLoadedScene->GetScene()
                                                  .GetProperties()
                                                  .GetSettings()
                                                  .GetSkyboxSettings()
                                                  .GetCubemapResolution();
      mState.SkyboxSettings.CubemapRotation = mLoadedScene->GetScene()
                                                .GetProperties()
                                                .GetSettings()
                                                .GetSkyboxSettings()
                                                .GetCubemapRotation();
      mState.AmbientSettings.Type = mLoadedScene->GetScene()
                                      .GetProperties()
                                      .GetSettings()
                                      .GetAmbientSettings()
                                      .GetAmbientSource();
      mState.AmbientSettings.Color = mLoadedScene->GetScene()
                                       .GetProperties()
                                       .GetSettings()
                                       .GetAmbientSettings()
                                       .GetAmbientLightColor();
      mState.AmbientSettings.Intensity = mLoadedScene->GetScene()
                                           .GetProperties()
                                           .GetSettings()
                                           .GetAmbientSettings()
                                           .GetAmbientIntensity();
      mState.AmbientSettings.Gradient = mLoadedScene->GetScene()
                                          .GetProperties()
                                          .GetSettings()
                                          .GetAmbientSettings()
                                          .GetGradient();
      mState.AmbientSettings.Cubemap = mLoadedScene->GetScene()
                                         .GetProperties()
                                         .GetSettings()
                                         .GetAmbientSettings()
                                         .GetCubeMap();
      mState.AmbientSettings.UseIBL = mLoadedScene->GetScene()
                                        .GetProperties()
                                        .GetSettings()
                                        .GetAmbientSettings()
                                        .GetUseIBL();
      mState.FogSettings.Type = mLoadedScene->GetScene()
                                  .GetProperties()
                                  .GetSettings()
                                  .GetFogSettings()
                                  .GetFogType();
      mState.FogSettings.Color = mLoadedScene->GetScene()
                                   .GetProperties()
                                   .GetSettings()
                                   .GetFogSettings()
                                   .GetColor();
      mState.FogSettings.Start = mLoadedScene->GetScene()
                                   .GetProperties()
                                   .GetSettings()
                                   .GetFogSettings()
                                   .GetStart();
      mState.FogSettings.End = mLoadedScene->GetScene()
                                 .GetProperties()
                                 .GetSettings()
                                 .GetFogSettings()
                                 .GetEnd();
      mState.FogSettings.Density = mLoadedScene->GetScene()
                                     .GetProperties()
                                     .GetSettings()
                                     .GetFogSettings()
                                     .GetDensity();
      mState.ShadowSettings.Enabled = mLoadedScene->GetScene()
                                        .GetProperties()
                                        .GetSettings()
                                        .GetShadowSettings()
                                        .GetEnabled();
      mState.ShadowSettings.Resolution = mLoadedScene->GetScene()
                                           .GetProperties()
                                           .GetSettings()
                                           .GetShadowSettings()
                                           .GetResolution();
      mState.ShadowSettings.CascadeCount = mLoadedScene->GetScene()
                                             .GetProperties()
                                             .GetSettings()
                                             .GetShadowSettings()
                                             .GetCascadeCount();
      mState.ShadowSettings.Distance = mLoadedScene->GetScene()
                                         .GetProperties()
                                         .GetSettings()
                                         .GetShadowSettings()
                                         .GetDistance();
      mState.ShadowSettings.SplitLambda = mLoadedScene->GetScene()
                                            .GetProperties()
                                            .GetSettings()
                                            .GetShadowSettings()
                                            .GetSplitLambda();
      mState.ShadowSettings.DepthBias = mLoadedScene->GetScene()
                                          .GetProperties()
                                          .GetSettings()
                                          .GetShadowSettings()
                                          .GetDepthBias();
      mState.ShadowSettings.SlopeScaledBias = mLoadedScene->GetScene()
                                                .GetProperties()
                                                .GetSettings()
                                                .GetShadowSettings()
                                                .GetSlopeScaledBias();
      mState.ShadowSettings.PcfSamples = mLoadedScene->GetScene()
                                           .GetProperties()
                                           .GetSettings()
                                           .GetShadowSettings()
                                           .GetPcfSamples();
      mState.AntiAliasingSettings.Type = mLoadedScene->GetScene()
                                           .GetProperties()
                                           .GetSettings()
                                           .GetAntiAliasingSettings()
                                           .GetAntiAliasingMethod();
      mState.AntiAliasingSettings.MsaaSamples = mLoadedScene->GetScene()
                                                  .GetProperties()
                                                  .GetSettings()
                                                  .GetAntiAliasingSettings()
                                                  .GetSamples();
      mState.TonemapType =
        mLoadedScene->GetScene().GetProperties().GetSettings().GetToneMapType();
      mState.ExposureSettings.Type = mLoadedScene->GetScene()
                                       .GetProperties()
                                       .GetSettings()
                                       .GetExposureSettings()
                                       .GetExposureType();
      mState.ExposureSettings.Exposure = mLoadedScene->GetScene()
                                           .GetProperties()
                                           .GetSettings()
                                           .GetExposureSettings()
                                           .GetExposure();
      mState.BloomSettings.Enabled = mLoadedScene->GetScene()
                                       .GetProperties()
                                       .GetSettings()
                                       .GetBloomSettings()
                                       .GetEnabled();
      mState.BloomSettings.Threshold = mLoadedScene->GetScene()
                                         .GetProperties()
                                         .GetSettings()
                                         .GetBloomSettings()
                                         .GetThreshold();
      mState.BloomSettings.Intensity = mLoadedScene->GetScene()
                                         .GetProperties()
                                         .GetSettings()
                                         .GetBloomSettings()
                                         .GetIntensity();
      mState.BloomSettings.Radius = mLoadedScene->GetScene()
                                      .GetProperties()
                                      .GetSettings()
                                      .GetBloomSettings()
                                      .GetRadius();
    }
  }

  void
  SceneSettingsWindow::OnSceneUnload()
  {
  }

  void
  SceneSettingsWindow::Deserialize(nlohmann::json moduleData)
  {
  }

  auto
  SceneSettingsWindow::Serialize() -> nlohmann::json
  {
    nlohmann::json serializedModule;

    serializedModule["id"] = GetUuid()->toString();
    serializedModule["type"] = static_cast<int>(GetModuleType());
    serializedModule["label"] = GetModuleName();

    return serializedModule;
  }
}