#include "Core/Asset/Database/AssetComponents.h"
#include "pch.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "SceneSettingsWindow.h"
#include "UI/DwarfUI.h"

namespace Dwarf
{
  SceneSettingsWindow::SceneSettingsWindow(
    std::shared_ptr<IDwarfLogger>   logger,
    std::shared_ptr<ILoadedScene>   loadedScene,
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : IGuiModule(ModuleLabel("Scene Settings"),
                 ModuleType(MODULE_TYPE::SCENE_SETTINGS),
                 ModuleID(std::make_shared<UUID>()))
    , mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
    , mAssetDatabase(std::move(assetDatabase))
  {
  }

  SceneSettingsWindow::SceneSettingsWindow(
    SerializedModule                serializedModule,
    std::shared_ptr<IDwarfLogger>   logger,
    std::shared_ptr<ILoadedScene>   loadedScene,
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : IGuiModule(ModuleLabel("Scene Settings"),
                 ModuleType(MODULE_TYPE::SCENE_SETTINGS),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
    , mAssetDatabase(std::move(assetDatabase))
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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(800, 500));

    if (!ImGui::Begin(GetIdentifier().c_str(), &mWindowOpened, windowFlags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      ImGui::PopStyleVar(2);
      return;
    }
    ImGui::PopStyleVar(2);

    // Multisampling

    // Tonemapping

    // Exposure

    // IBL contribution

    // Shadow mapping settings

    // Anisotropic filtering

    if (ImGui::CollapsingHeader("Environment & Atmosphere"))
    {
      // Skybox material
      ImGui::SeparatorText("Skybox");
      // ImGui::Text("Skybox material");
      DwarfUI::AssetInput<MaterialAsset>(mAssetDatabase,
                                         mLoadedScene->GetScene()
                                           .GetProperties()
                                           .GetSettings()
                                           .GetSkyboxMaterial(),
                                         "skyboxMaterial");

      // Ambient light color / intensity
      ImGui::SeparatorText("Ambient Light");
      ImGui::ColorPicker3("Light Color",
                          glm::value_ptr(mLoadedScene->GetScene()
                                           .GetProperties()
                                           .GetSettings()
                                           .GetAmbientLightSettings()
                                           .color));
      ImGui::DragFloat("Light Intensity",
                       &mLoadedScene->GetScene()
                          .GetProperties()
                          .GetSettings()
                          .GetAmbientLightSettings()
                          .intensity,
                       0.5F,
                       0.0F,
                       +FLT_MAX,
                       "%.3f");

      // Fog settings
      ImGui::SeparatorText("Fog Settings");
      ImGui::ColorPicker3("Fog Color",
                          glm::value_ptr(mLoadedScene->GetScene()
                                           .GetProperties()
                                           .GetSettings()
                                           .GetFogSettings()
                                           .Color));
      DwarfUI::ComboEnum<FogType>("Fog Type",
                                  mLoadedScene->GetScene()
                                    .GetProperties()
                                    .GetSettings()
                                    .GetFogSettings()
                                    .Type);

      switch (mLoadedScene->GetScene()
                .GetProperties()
                .GetSettings()
                .GetFogSettings()
                .Type)
      {
        using enum FogType;
        case LINEAR:
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
                                 100.0f,
                                 "Start: %.1f %%",
                                 "End: %.1f %%",
                                 ImGuiSliderFlags_AlwaysClamp);
          break;
        case EXPONENTIAL:
          ImGui::DragFloat("Fog Density",
                           &mLoadedScene->GetScene()
                              .GetProperties()
                              .GetSettings()
                              .GetFogSettings()
                              .Density,
                           0.005f,
                           0.0f,
                           FLT_MAX,
                           "%.3f",
                           0);
          break;
      }

      // Global exposure
      ImGui::SeparatorText("Global Exposure");

      // Tonemapping
      ImGui::SeparatorText("Tonemapping");

      // HDRI for IBL
      ImGui::SeparatorText("Fog Settings");
    }

    if (ImGui::CollapsingHeader("Lighting"))
    {
      // Global shadow settings

      // Light Probe Grid (Future)
    }

    if (ImGui::CollapsingHeader("Rendering & Post-Processing"))
    {
      // Anti-Aliasing Method
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