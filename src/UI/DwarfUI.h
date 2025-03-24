#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/GenericComponents.h"
#include <entt/entt.hpp>
#include <imgui.h>

#define COL_BLACK IM_COL32(0, 0, 0, 0)
#define COL_DIM2 IM_COL32(59, 66, 82, 255)
#define COL_MID IM_COL32(76, 86, 106, 255)
#define COL_BRIGHT IM_COL32(129, 161, 193, 255)

#define COL_BUTTON IM_COL32(76, 86, 106, 255)
#define COL_BUTTON_HOVERED IM_COL32(86, 95, 114, 255)
#define COL_BUTTON_ACTIVE IM_COL32(129, 161, 193, 255)

#define COL_POPUP_BG IM_COL32(46 * 0.8, 52 * 0.8, 64 * 0.8, 255)
#define COL_BG_DIM IM_COL32(46, 52, 64, 255)
#define COL_BG_BRIGHT IM_COL32(76, 86, 106, 255)

#define COL_TEXT IM_COL32(229, 233, 240, 255)
#define COL_SELECTED IM_COL32(76, 86, 106, 255)

#define COL_WINDOW_HEADER_FOCUSED IM_COL32(46, 52, 64, 255)
#define COL_WINDOW_HEADER_UNFOCUSED                                            \
  IM_COL32(46 * 0.7F, 52 * 0.7F, 64 * 0.7F, 255)

#define COL_TAB IM_COL32(59, 66, 82, 255)
#define COL_TAB_ACTIVE IM_COL32(76, 86, 106, 255)
#define COL_TAB_HOVERED IM_COL32(86, 95, 114, 255)

#define COL_TAB_UNFOCUSED IM_COL32(59, 66, 82, 255)
#define COL_TAB_UNFOCUSED_ACTIVE IM_COL32(76, 86, 106, 255)

#define COMPONENT_PANEL_PADDING (8.0f)
#define ADD_BUTTON_WIDTH (40.0f)
#define UNIFORM_DELETE_BUTTON_WIDTH (80.0f)

namespace Dwarf
{
  class DwarfUI
  {
  public:
    static void
    TextButton(const char* text, ImVec2 buttonSize);
    static void
    ImageButton(ImTextureID texID, ImVec2 buttonSize, ImVec2 imageSize);
    static void
    InputField();
    static void
    Text();

    static auto
    InputTextCallback(ImGuiInputTextCallbackData* data) -> int;

    template<typename T>
    static void
    AssetInput(const std::shared_ptr<IAssetDatabase>& assetDatabase,
               std::unique_ptr<IAssetReference>&      assetRef,
               const char*                            imguiID)
    {
      std::vector<entt::entity> availableAssets;
      int                       selectedAsset = -1;
      const auto&               view =
        assetDatabase->GetRegistry().view<IDComponent, NameComponent, T>();

      int count = 0;
      for (auto entity : view)
      {
        availableAssets.push_back(entity);
        if (assetRef && (view.template get<IDComponent>(entity).getId() ==
                         assetRef->GetUID()))
        {
          selectedAsset = count;
        }
        count++;
      }

      const char* preview =
        (selectedAsset == -1)
          ? "None"
          : view.template get<NameComponent>(availableAssets[selectedAsset])
              .Name.c_str();

      if (ImGui::BeginCombo(imguiID, preview))
      {
        if (ImGui::Selectable(
              "None", selectedAsset == -1, 0, ImVec2(0, 16 + 10)))
        {
          selectedAsset = -1;
          assetRef = nullptr;
        }

        for (int i = 0; i < availableAssets.size(); i++)
        {
          const bool isSelected = (selectedAsset == i);
          if (ImGui::Selectable(
                view.template get<NameComponent>(availableAssets[i])
                  .Name.c_str(),
                isSelected,
                0,
                ImVec2(0, 16 + 10)))
          {
            selectedAsset = i;
            assetRef = assetDatabase->Retrieve(
              view.template get<IDComponent>(availableAssets[i]).getId());
          }
        }

        ImGui::EndCombo();
      }
    }

    template<typename T>
    static void
    AssetInput(const std::shared_ptr<IAssetDatabase>&           assetDatabase,
               std::optional<std::unique_ptr<IAssetReference>>& assetRef,
               const char*                                      imguiID)
    {
      std::vector<entt::entity> availableAssets;
      int                       selectedAsset = -1;
      const auto&               view =
        assetDatabase->GetRegistry().view<IDComponent, NameComponent, T>();

      int count = 0;
      for (auto entity : view)
      {
        availableAssets.push_back(entity);
        if (assetRef.has_value() && assetRef.value() != nullptr &&
            (view.template get<IDComponent>(entity).getId() ==
             assetRef.value()->GetUID()))
        {
          selectedAsset = count;
        }
        count++;
      }

      const char* preview =
        (selectedAsset == -1)
          ? "None"
          : view.template get<NameComponent>(availableAssets[selectedAsset])
              .Name.c_str();

      if (ImGui::BeginCombo(imguiID, preview))
      {
        if (ImGui::Selectable(
              "None", selectedAsset == -1, 0, ImVec2(0, 16 + 10)))
        {
          selectedAsset = -1;
          assetRef = std::nullopt;
        }

        for (int i = 0; i < availableAssets.size(); i++)
        {
          const bool isSelected = (selectedAsset == i);
          if (ImGui::Selectable(
                view.template get<NameComponent>(availableAssets[i])
                  .Name.c_str(),
                isSelected,
                0,
                ImVec2(0, 16 + 10)))
          {
            selectedAsset = i;
            assetRef = assetDatabase->Retrieve(
              view.template get<IDComponent>(availableAssets[i]).getId());
          }
        }

        ImGui::EndCombo();
      }
    }

    template<typename T>
    static void
    AssetInput(const std::shared_ptr<IAssetDatabase>& assetDatabase,
               std::optional<UUID>&                   assetRef,
               const char*                            imguiID)
    {
      std::vector<entt::entity> availableAssets;
      int                       selectedAsset = -1;
      auto                      view =
        assetDatabase->GetRegistry().view<IDComponent, NameComponent, T>();

      int count = 0;
      for (auto entity : view)
      {
        availableAssets.push_back(entity);
        if (assetRef.has_value() &&
            (view.template get<IDComponent>(entity).getId() ==
             assetRef.value()))
        {
          selectedAsset = count;
        }
        count++;
      }

      const char* preview =
        (selectedAsset == -1)
          ? "None"
          : view.template get<NameComponent>(availableAssets[selectedAsset])
              .Name.c_str();

      if (ImGui::BeginCombo(imguiID, preview))
      {
        if (ImGui::Selectable(
              "None", selectedAsset == -1, 0, ImVec2(0, 16 + 10)))
        {
          selectedAsset = -1;
          assetRef = std::nullopt;
        }

        for (int i = 0; i < availableAssets.size(); i++)
        {
          const bool isSelected = (selectedAsset == i);
          if (ImGui::Selectable(
                view.template get<NameComponent>(availableAssets[i])
                  .Name.c_str(),
                isSelected,
                0,
                ImVec2(0, 16 + 10)))
          {
            selectedAsset = i;
            assetRef =
              view.template get<IDComponent>(availableAssets[i]).getId();
          }
        }

        ImGui::EndCombo();
      }
    }

    template<>
    void
    AssetInput<VertexShaderAsset>(
      const std::shared_ptr<IAssetDatabase>&           assetDatabase,
      std::optional<std::unique_ptr<IAssetReference>>& assetRef,
      const char*                                      imguiID);

    template<>
    void
    AssetInput<FragmentShaderAsset>(
      const std::shared_ptr<IAssetDatabase>&           assetDatabase,
      std::optional<std::unique_ptr<IAssetReference>>& assetRef,
      const char*                                      imguiID);
  };
}