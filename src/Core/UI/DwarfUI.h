#pragma once

#include "Core/Base.h"
#include "Core/GenericComponents.h"
#include "Core/Scene/SceneComponents.h"
#include <imgui.h>
#include <entt/entt.hpp>
#include <Core/Asset/AssetDatabase.h>

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
#define COL_WINDOW_HEADER_UNFOCUSED IM_COL32(46 * 0.7f, 52 * 0.7f, 64 * 0.7f, 255)

#define COL_TAB IM_COL32(59, 66, 82, 255)
#define COL_TAB_ACTIVE IM_COL32(76, 86, 106, 255)
#define COL_TAB_HOVERED IM_COL32(86, 95, 114, 255)

#define COL_TAB_UNFOCUSED IM_COL32(59, 66, 82, 255)
#define COL_TAB_UNFOCUSED_ACTIVE IM_COL32(76, 86, 106, 255)

namespace Dwarf
{
    class DwarfUI
    {
    public:
        static void TextButton(const char *text, ImVec2 buttonSize);
        static void ImageButton(ImTextureID texID, ImVec2 buttonSize, ImVec2 imageSize);
        static void InputField();
        static void Text();

        template <typename T>
        static void AssetInput(Ref<UID> &assetID, const char *imguiID)
        {
            std::vector<entt::entity> availableAssets;
            int selectedAsset = -1;
            auto view = AssetDatabase::AssetDatabase::s_Registry->view<IDComponent, NameComponent, T>();

            int count = 0;
            for (auto entity : view)
            {
                availableAssets.push_back(entity);
                if (assetID && (*view.template get<IDComponent>(entity).ID == *assetID))
                {
                    selectedAsset = count;
                }
                count++;
            }

            const char *preview = (selectedAsset == -1) ? "None" : view.template get<NameComponent>(availableAssets[selectedAsset]).Name.c_str();

            if (ImGui::BeginCombo(imguiID, preview))
            {
                if (ImGui::Selectable("None", selectedAsset == -1, 0, ImVec2(0, 16 + 10)))
                {
                    selectedAsset = -1;
                    assetID = nullptr;
                }

                for (int i = 0; i < availableAssets.size(); i++)
                {
                    const bool is_selected = (selectedAsset == i);
                    if (ImGui::Selectable(view.template get<NameComponent>(availableAssets[i]).Name.c_str(), is_selected, 0, ImVec2(0, 16 + 10)))
                    {
                        selectedAsset = i;
                        assetID = view.template get<IDComponent>(availableAssets[i]).ID;
                    }
                }

                ImGui::EndCombo();
            }
        }
    };

    template <>
    void DwarfUI::AssetInput<VertexShaderAsset>(Ref<UID> &assetID, const char *imguiID);

    template <>
    void DwarfUI::AssetInput<FragmentShaderAsset>(Ref<UID> &assetID, const char *imguiID);
}