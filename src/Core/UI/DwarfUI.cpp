#include "dpch.h"

#include "DwarfUI.h"

namespace Dwarf {
  template<>
  void DwarfUI::AssetInput<VertexShaderAsset>(Ref<UID>& assetID,
                                              const char* imguiID)
  {
    std::vector<entt::entity> availableAssets;
    int selectedAsset = -1;
    auto view = AssetDatabase::AssetDatabase::s_Registry
                  ->view<IDComponent, NameComponent, VertexShaderAsset>();

    int count = 0;
    for (auto entity : view) {
      availableAssets.push_back(entity);
      if (assetID && (*view.template get<IDComponent>(entity).ID == *assetID)) {
        selectedAsset = count;
      }
      count++;
    }

    const char* preview =
      (selectedAsset == -1)
        ? "Default"
        : view.template get<NameComponent>(availableAssets[selectedAsset])
            .Name.c_str();

    if (ImGui::BeginCombo(imguiID, preview)) {
      if (ImGui::Selectable(
            "Default", selectedAsset == -1, 0, ImVec2(0, 16 + 10))) {
        selectedAsset = -1;
        assetID = nullptr;
      }

      for (int i = 0; i < availableAssets.size(); i++) {
        const bool is_selected = (selectedAsset == i);
        if (ImGui::Selectable(
              view.template get<NameComponent>(availableAssets[i]).Name.c_str(),
              is_selected,
              0,
              ImVec2(0, 16 + 10))) {
          selectedAsset = i;
          assetID = view.template get<IDComponent>(availableAssets[i]).ID;
        }
      }

      ImGui::EndCombo();
    }
  }

  template<>
  void DwarfUI::AssetInput<FragmentShaderAsset>(Ref<UID>& assetID,
                                                const char* imguiID)
  {
    std::vector<entt::entity> availableAssets;
    int selectedAsset = -1;
    auto view = AssetDatabase::AssetDatabase::s_Registry
                  ->view<IDComponent, NameComponent, FragmentShaderAsset>();

    int count = 0;
    for (auto entity : view) {
      availableAssets.push_back(entity);
      if (assetID && (*view.template get<IDComponent>(entity).ID == *assetID)) {
        selectedAsset = count;
      }
      count++;
    }

    const char* preview =
      (selectedAsset == -1)
        ? "Default"
        : view.template get<NameComponent>(availableAssets[selectedAsset])
            .Name.c_str();

    if (ImGui::BeginCombo(imguiID, preview)) {
      if (ImGui::Selectable(
            "Default", selectedAsset == -1, 0, ImVec2(0, 16 + 10))) {
        selectedAsset = -1;
        assetID = nullptr;
      }

      for (int i = 0; i < availableAssets.size(); i++) {
        const bool is_selected = (selectedAsset == i);
        if (ImGui::Selectable(
              view.template get<NameComponent>(availableAssets[i]).Name.c_str(),
              is_selected,
              0,
              ImVec2(0, 16 + 10))) {
          selectedAsset = i;
          assetID = view.template get<IDComponent>(availableAssets[i]).ID;
        }
      }

      ImGui::EndCombo();
    }
  }
}