#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/GenericComponents.h"
#include <algorithm>
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
    static auto
    AssetInput(const std::shared_ptr<IAssetDatabase>& assetDatabase,
               std::unique_ptr<IAssetReference>&      assetRef,
               const char*                            imguiID) -> bool
    {
      std::vector<entt::entity> availableAssets;
      auto                      view =
        assetDatabase->GetRegistry().view<IDComponent, NameComponent, T>();
      static bool interacted = false;

      for (auto entity : view)
      {
        availableAssets.push_back(entity);
      }

      const char* preview =
        (assetRef == nullptr)
          ? "None"
          : view.template get<NameComponent>(assetRef->GetHandle())
              .Name.c_str();

      std::ranges::sort(
        availableAssets,
        [view](entt::entity& first, entt::entity& second)
        {
          return to_lower(view.template get<NameComponent>(first).Name) <
                 to_lower(view.template get<NameComponent>(second).Name);
        });

      interacted = false;
      if (ImGui::BeginCombo(imguiID, preview))
      {
        if (ImGui::Selectable(
              "None", assetRef == nullptr, 0, ImVec2(0, 16 + 10)))
        {
          assetRef = nullptr;
          interacted = true;
        }

        for (auto& assetHandle : availableAssets)
        {
          // const bool isSelected = (selectedAsset == i);
          if (ImGui::Selectable(
                view.template get<NameComponent>(assetHandle).Name.c_str(),
                (assetRef != nullptr) && (assetRef->GetHandle() == assetHandle),
                0,
                ImVec2(0, 16 + 10)))
          {
            assetRef = assetDatabase->Retrieve(
              view.template get<IDComponent>(assetHandle).getId());
            interacted = true;
          }
        }

        ImGui::EndCombo();
      }

      return interacted;
    }

    template<typename T>
    static auto
    AssetInput(const std::shared_ptr<IAssetDatabase>&           assetDatabase,
               std::optional<std::unique_ptr<IAssetReference>>& assetRef,
               const char*                                      imguiID) -> bool
    {
      std::vector<entt::entity> availableAssets;
      auto                      view =
        assetDatabase->GetRegistry().view<IDComponent, NameComponent, T>();
      static bool interacted = false;

      for (auto entity : view)
      {
        availableAssets.push_back(entity);
      }

      bool hasValue = assetRef.has_value() && (assetRef.value() != nullptr);

      const char* preview =
        !hasValue
          ? "None"
          : view.template get<NameComponent>(assetRef.value()->GetHandle())
              .Name.c_str();

      std::ranges::sort(
        availableAssets,
        [view](entt::entity& first, entt::entity& second)
        {
          return to_lower(view.template get<NameComponent>(first).Name) <
                 to_lower(view.template get<NameComponent>(second).Name);
        });

      interacted = false;
      if (ImGui::BeginCombo(imguiID, preview))
      {
        if (ImGui::Selectable("None", !hasValue, 0, ImVec2(0, 16 + 10)))
        {
          assetRef = std::nullopt;
          interacted = true;
        }

        for (auto& assetHandle : availableAssets)
        {
          if (ImGui::Selectable(
                view.template get<NameComponent>(assetHandle).Name.c_str(),
                hasValue && assetRef.value()->GetHandle() == assetHandle,
                0,
                ImVec2(0, 16 + 10)))
          {
            assetRef = assetDatabase->Retrieve(
              view.template get<IDComponent>(assetHandle).getId());
            interacted = true;
          }
        }

        ImGui::EndCombo();
      }
      return interacted;
    }

    template<typename T>
    static auto
    AssetInput(const std::shared_ptr<IAssetDatabase>& assetDatabase,
               std::optional<UUID>&                   assetRef,
               const char*                            imguiID) -> bool
    {
      std::vector<entt::entity> availableAssets;
      auto                      view =
        assetDatabase->GetRegistry().view<IDComponent, NameComponent, T>();
      static bool interacted = false;

      for (auto entity : view)
      {
        availableAssets.push_back(entity);
      }

      bool hasValue = assetRef.has_value();

      std::ranges::sort(
        availableAssets,
        [view](entt::entity& first, entt::entity& second)
        {
          return to_lower(view.template get<NameComponent>(first).Name) <
                 to_lower(view.template get<NameComponent>(second).Name);
        });

      auto selectedIterator = std::ranges::find_if(
        availableAssets,
        [view, assetRef, hasValue](entt::entity& handle)
        {
          return hasValue ? view.template get<IDComponent>(handle).getId() ==
                              assetRef.value()
                          : false;
        });

      const char* preview =
        (!hasValue || (selectedIterator == availableAssets.end()))
          ? "None"
          : view.template get<NameComponent>(*selectedIterator).Name.c_str();

      interacted = false;
      if (ImGui::BeginCombo(imguiID, preview))
      {
        if (ImGui::Selectable("None",
                              (!assetRef.has_value() ||
                               (selectedIterator == availableAssets.end())),
                              0,
                              ImVec2(0, 16 + 10)))
        {
          assetRef = std::nullopt;
          interacted = true;
        }

        for (auto& availableAsset : availableAssets)
        {
          // const bool isSelected = (selectedAsset == i);
          if (ImGui::Selectable(
                view.template get<NameComponent>(availableAsset).Name.c_str(),
                hasValue &&
                  (assetRef.value() ==
                   view.template get<IDComponent>(availableAsset).getId()),
                0,
                ImVec2(0, 16 + 10)))
          {
            assetRef = view.template get<IDComponent>(availableAsset).getId();
            interacted = true;
          }
        }

        ImGui::EndCombo();
      }

      return interacted;
    }
  };
}