#include "DwarfUI.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Platform/OpenGL/OpenGLUtilities.h"

namespace Dwarf
{
  template<>
  void
  DwarfUI::AssetInput<VertexShaderAsset>(
    std::shared_ptr<IAssetDatabase>                  assetDatabase,
    std::optional<std::unique_ptr<IAssetReference>>& assetRef,
    const char*                                      imguiID)
  {
    // TODO: Make more efficient
    std::vector<entt::entity> availableAssets;
    int                       selectedAsset = -1;
    auto                      view = assetDatabase->GetRegistry()
                  .view<IDComponent, NameComponent, VertexShaderAsset>();

    int count = 0;
    // Compile list of available assets
    for (auto entity : view)
    {
      availableAssets.push_back(entity);
      // Check if assetRef is set, and if so, set selectedAsset
      if (assetRef && (view.template get<IDComponent>(entity).GetID() ==
                       assetRef.value()->GetUID()))
      {
        selectedAsset = count;
      }
      count++;
    }

    const char* preview =
      view.template get<NameComponent>(availableAssets[selectedAsset])
        .Name.c_str();

    if (ImGui::BeginCombo(imguiID, preview))
    {
      for (int i = 0; i < availableAssets.size(); i++)
      {
        const bool is_selected = (selectedAsset == i);
        if (ImGui::Selectable(
              view.template get<NameComponent>(availableAssets[i]).Name.c_str(),
              is_selected,
              0,
              ImVec2(0, 16 + 10)))
        {
          selectedAsset = i;
          assetRef = assetDatabase->Retrieve(
            view.template get<IDComponent>(availableAssets[i]).GetID());
        }
      }

      ImGui::EndCombo();
    }
  }

  template<>
  void
  DwarfUI::AssetInput<FragmentShaderAsset>(
    std::shared_ptr<IAssetDatabase>                  assetDatabase,
    std::optional<std::unique_ptr<IAssetReference>>& assetRef,
    const char*                                      imguiID)
  {
    // TODO: Make more efficient
    std::vector<entt::entity> availableAssets;
    int                       selectedAsset = -1;
    auto                      view = assetDatabase->GetRegistry()
                  .view<IDComponent, NameComponent, FragmentShaderAsset>();

    int count = 0;
    // Compile list of available assets
    for (auto entity : view)
    {
      availableAssets.push_back(entity);
      // Check if assetRef is set, and if so, set selectedAsset
      if (assetRef && (view.template get<IDComponent>(entity).GetID() ==
                       assetRef.value()->GetUID()))
      {
        selectedAsset = count;
      }
      count++;
    }

    const char* preview =
      view.template get<NameComponent>(availableAssets[selectedAsset])
        .Name.c_str();

    if (ImGui::BeginCombo(imguiID, preview))
    {
      for (int i = 0; i < availableAssets.size(); i++)
      {
        const bool is_selected = (selectedAsset == i);
        if (ImGui::Selectable(
              view.template get<NameComponent>(availableAssets[i]).Name.c_str(),
              is_selected,
              0,
              ImVec2(0, 16 + 10)))
        {
          selectedAsset = i;
          assetRef = assetDatabase->Retrieve(
            view.template get<IDComponent>(availableAssets[i]).GetID());
        }
      }

      ImGui::EndCombo();
    }
  }
}