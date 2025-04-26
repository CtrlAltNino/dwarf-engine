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

    if (ImGui::CollapsingHeader("Environment & Atmosphere"))
    {
      // Skybox material
      ImGui::SeparatorText("Skybox");
      DwarfUI::ComboEnum("Source##SkyboxSource",
                         mLoadedScene->GetScene()
                           .GetProperties()
                           .GetSettings()
                           .GetSkyboxSettings()
                           .Type);

      switch (mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetSkyboxSettings()
                .Type)
      {
        using enum SkyboxSource;
        case Color:
          ImGui::ColorEdit3("Color##SkyboxColor",
                            glm::value_ptr(mLoadedScene->GetScene()
                                             .GetProperties()
                                             .GetSettings()
                                             .GetSkyboxSettings()
                                             .Color));
          break;
        case Material:
          DwarfUI::AssetInput<MaterialAsset>(mAssetDatabase,
                                             mLoadedScene->GetScene()
                                               .GetProperties()
                                               .GetSettings()
                                               .GetSkyboxSettings()
                                               .SkyboxMaterial,
                                             "Material##SkyboxMaterial");
          break;
        case HDRI:
          DwarfUI::AssetInput<TextureAsset>(mAssetDatabase,
                                            mLoadedScene->GetScene()
                                              .GetProperties()
                                              .GetSettings()
                                              .GetSkyboxSettings()
                                              .CubeMap,
                                            "HDRI##SkyboxCubeMap");
          break;
      }

      // Ambient light color / intensity
      ImGui::SeparatorText("Ambient Lighting Settings");
      DwarfUI::ComboEnum<AmbientSource>("Source##AmbientSource",
                                        mLoadedScene->GetScene()
                                          .GetProperties()
                                          .GetSettings()
                                          .GetAmbientSettings()
                                          .Type);

      switch (mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetAmbientSettings()
                .Type)
      {
        using enum AmbientSource;
        case None:
        case Skybox: break;
        case Color:
          ImGui::ColorEdit3("Color##AmbientColor",
                            glm::value_ptr(mLoadedScene->GetScene()
                                             .GetProperties()
                                             .GetSettings()
                                             .GetAmbientSettings()
                                             .Color));
          ImGui::DragFloat("Intensity",
                           &mLoadedScene->GetScene()
                              .GetProperties()
                              .GetSettings()
                              .GetAmbientSettings()
                              .Intensity,
                           0.2F,
                           0.0F,
                           +FLT_MAX,
                           "%.3f");
          break;
        case Gradient:
          ImGui::ColorEdit3("Sky",
                            glm::value_ptr(mLoadedScene->GetScene()
                                             .GetProperties()
                                             .GetSettings()
                                             .GetAmbientSettings()
                                             .Gradient[0]));
          ImGui::ColorEdit3("Horizon",
                            glm::value_ptr(mLoadedScene->GetScene()
                                             .GetProperties()
                                             .GetSettings()
                                             .GetAmbientSettings()
                                             .Gradient[1]));
          ImGui::ColorEdit3("Ground",
                            glm::value_ptr(mLoadedScene->GetScene()
                                             .GetProperties()
                                             .GetSettings()
                                             .GetAmbientSettings()
                                             .Gradient[2]));
          ImGui::DragFloat("Intensity##AmbientIntensity",
                           &mLoadedScene->GetScene()
                              .GetProperties()
                              .GetSettings()
                              .GetAmbientSettings()
                              .Intensity,
                           0.2F,
                           0.0F,
                           10.0F);
          break;
        case HDRI:
          DwarfUI::AssetInput<TextureAsset>(mAssetDatabase,
                                            mLoadedScene->GetScene()
                                              .GetProperties()
                                              .GetSettings()
                                              .GetAmbientSettings()
                                              .CubeMap,
                                            "HDRI##AmbientCubeMap");
          ImGui::Checkbox("Use for IBL",
                          &mLoadedScene->GetScene()
                             .GetProperties()
                             .GetSettings()
                             .GetAmbientSettings()
                             .UseIBL);
          break;
      }

      // Fog settings
      ImGui::SeparatorText("Fog Settings");
      DwarfUI::ComboEnum<FogType>("Fog Type",
                                  mLoadedScene->GetScene()
                                    .GetProperties()
                                    .GetSettings()
                                    .GetFogSettings()
                                    .Type);
      ImGui::ColorEdit3("Fog Color",
                        glm::value_ptr(mLoadedScene->GetScene()
                                         .GetProperties()
                                         .GetSettings()
                                         .GetFogSettings()
                                         .Color));

      switch (mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetFogSettings()
                .Type)
      {
        using enum FogType;
        case Linear:
          ImGui::DragFloatRange2("Fog Distance",
                                 &mLoadedScene->GetScene()
                                    .GetProperties()
                                    .GetSettings()
                                    .GetFogSettings()
                                    .Start,
                                 &mLoadedScene->GetScene()
                                    .GetProperties()
                                    .GetSettings()
                                    .GetFogSettings()
                                    .End,
                                 0.25f,
                                 0.0f,
                                 +FLT_MAX,
                                 "Start: %.1f",
                                 "End: %.1f",
                                 ImGuiSliderFlags_AlwaysClamp);
          break;
        case Exponential:
          ImGui::DragFloat("Fog Density",
                           &mLoadedScene->GetScene()
                              .GetProperties()
                              .GetSettings()
                              .GetFogSettings()
                              .Density,
                           0.005f,
                           0.0f,
                           +FLT_MAX,
                           "%.3f",
                           0);
          break;
      }
    }

    if (ImGui::CollapsingHeader("Lighting"))
    {
      // Global shadow settings
      ImGui::SeparatorText("Shadow Settings");

      // Enable/disable shadow mapping
      ImGui::Checkbox("Enable Shadows",
                      &mLoadedScene->GetScene()
                         .GetProperties()
                         .GetSettings()
                         .GetShadowSettings()
                         .Enabled);

      // Shadow map resolution
      DwarfUI::ComboEnum<ShadowMapResolution>("Shadow Map Resolution",
                                              mLoadedScene->GetScene()
                                                .GetProperties()
                                                .GetSettings()
                                                .GetShadowSettings()
                                                .Resolution);

      // Number of cascades (clamped between 1–4 typically)
      static uint8_t minCascades = 1U;
      static uint8_t maxCascades = 4U;
      ImGui::SliderScalar("Cascade Count",
                          ImGuiDataType_U8,
                          &mLoadedScene->GetScene()
                             .GetProperties()
                             .GetSettings()
                             .GetShadowSettings()
                             .CascadeCount,
                          &minCascades,
                          &maxCascades);

      // Shadow distance (far plane for cascades)
      ImGui::DragFloat("Shadow Distance",
                       &mLoadedScene->GetScene()
                          .GetProperties()
                          .GetSettings()
                          .GetShadowSettings()
                          .Distance,
                       10.0f,
                       10.0f,
                       +FLT_MAX,
                       "%.1f m");

      // Split scheme factor (lambda between uniform and log split)
      ImGui::SliderFloat("Cascade Split Lambda",
                         &mLoadedScene->GetScene()
                            .GetProperties()
                            .GetSettings()
                            .GetShadowSettings()
                            .SplitLambda,
                         0.0f,
                         1.0f);

      // Bias
      ImGui::SliderFloat("Depth Bias",
                         &mLoadedScene->GetScene()
                            .GetProperties()
                            .GetSettings()
                            .GetShadowSettings()
                            .DepthBias,
                         0.0F,
                         0.01F,
                         "%.5f");
      ImGui::SliderFloat("Slope Bias",
                         &mLoadedScene->GetScene()
                            .GetProperties()
                            .GetSettings()
                            .GetShadowSettings()
                            .SlopeScaledBias,
                         0.0F,
                         5.0F,
                         "%.2f");

      // Filtering
      static uint8_t minPfcSamples = 1;
      static uint8_t maxPfcSamples = 64;
      ImGui::SliderScalar("PCF Samples",
                          ImGuiDataType_U8,
                          &mLoadedScene->GetScene()
                             .GetProperties()
                             .GetSettings()
                             .GetShadowSettings()
                             .PcfSamples,
                          &minPfcSamples,
                          &maxPfcSamples);

      // Visual debug toggle
      // ImGui::Checkbox("Visualize Cascades", &settings.visualizeCascades);
    }

    if (ImGui::CollapsingHeader("Rendering & Post-Processing"))
    {
      ImGui::SeparatorText("Anti-Aliasing");
      // Anti-Aliasing Method
      static AntiAliasingMethod antiAliasingMethod =
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetAntiAliasingSettings()
          .GetAntiAliasingMethod();
      if (DwarfUI::ComboEnum<AntiAliasingMethod>("Anti-Aliasing Method",
                                                 antiAliasingMethod))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetAntiAliasingSettings()
          .SetAntiAliasingMethod(antiAliasingMethod);
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
          static uint8_t samples = mLoadedScene->GetScene()
                                     .GetProperties()
                                     .GetSettings()
                                     .GetAntiAliasingSettings()
                                     .GetSamples();
          std::string format = std::format("{}/{}", "%d", max);

          if (ImGui::SliderScalar("MSAA Samples",
                                  ImGuiDataType_U8,
                                  &samples,
                                  &min,
                                  &max,
                                  format.c_str(),
                                  ImGuiSliderFlags_None))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetAntiAliasingSettings()
              .SetSamples(samples);
          }
          break;
      }

      // Tonemapping
      ImGui::SeparatorText("Tonemapping");
      static TonemapType type =
        mLoadedScene->GetScene().GetProperties().GetSettings().GetToneMapType();
      if (DwarfUI::ComboEnum<TonemapType>("Tonemapping", type))
      {
        mLoadedScene->GetScene().GetProperties().GetSettings().SetToneMapType(
          type);
      }

      // Global exposure
      ImGui::SeparatorText("Global Exposure");
      static ExposureType exposureType = mLoadedScene->GetScene()
                                           .GetProperties()
                                           .GetSettings()
                                           .GetExposureSettings()
                                           .GetExposureType();
      if (DwarfUI::ComboEnum<ExposureType>("Exposure Type", exposureType))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetExposureSettings()
          .SetExposureType(exposureType);
      }

      switch (mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetExposureSettings()
                .GetExposureType())
      {
        using enum ExposureType;
        case Manual:
          static float exposureValue = mLoadedScene->GetScene()
                                         .GetProperties()
                                         .GetSettings()
                                         .GetExposureSettings()
                                         .GetExposure();
          if (ImGui::DragFloat(
                "Exposure", &exposureValue, 0.0005f, 0.0F, 20.0F, "%f"))
          {
            mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetExposureSettings()
              .SetExposure(exposureValue);
          }
          break;
        case Automatic:
          // TODO: Implement automatic exposure system
          break;
      }

      // Bloom Settings
      ImGui::SeparatorText("Bloom");

      // Toggle bloom enabled status
      static bool bloomEnabled = mLoadedScene->GetScene()
                                   .GetProperties()
                                   .GetSettings()
                                   .GetBloomSettings()
                                   .GetEnabled();
      if (ImGui::Checkbox("Enable", &bloomEnabled))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetBloomSettings()
          .SetEnabled(bloomEnabled);
      }

      // Bloom Threshold
      static float bloomThreshold = mLoadedScene->GetScene()
                                      .GetProperties()
                                      .GetSettings()
                                      .GetBloomSettings()
                                      .GetThreshold();
      if (ImGui::DragFloat(
            "Threshold", &bloomThreshold, 0.01F, 0.0F, +FLT_MAX, "%.2f"))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetBloomSettings()
          .SetThreshold(bloomThreshold);
      }

      static float bloomIntensity = mLoadedScene->GetScene()
                                      .GetProperties()
                                      .GetSettings()
                                      .GetBloomSettings()
                                      .GetIntensity();
      if (ImGui::DragFloat(
            "Intensity", &bloomIntensity, 0.01F, 0.0F, +FLT_MAX, "%.2f"))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetBloomSettings()
          .SetIntensity(bloomIntensity);
      }

      static float bloomRadius = mLoadedScene->GetScene()
                                   .GetProperties()
                                   .GetSettings()
                                   .GetBloomSettings()
                                   .GetRadius();
      if (ImGui::DragFloat(
            "Radius", &bloomRadius, 0.01F, 0.0F, +FLT_MAX, "%.2f"))
      {
        mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetBloomSettings()
          .SetRadius(bloomRadius);
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

    ImGui::End();
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