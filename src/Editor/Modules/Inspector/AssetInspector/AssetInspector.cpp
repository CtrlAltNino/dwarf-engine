#include "pch.h"

#include "AssetInspector.h"

namespace Dwarf
{
  AssetInspector::AssetInspector(
    std::shared_ptr<IAssetDatabase>          assetDatabase,
    std::shared_ptr<IMaterialAssetInspector> materialAssetInspector,
    std::shared_ptr<ISceneAssetInspector>    sceneAssetInspector,
    std::shared_ptr<IModelAssetInspector>    modelAssetInspector,
    std::shared_ptr<ITextureAssetInspector>  textureAssetInspector,
    std::shared_ptr<IEditorSelection>        editorSelection)
    : mAssetDatabase(std::move(assetDatabase))
    , mMaterialAssetInspector(std::move(materialAssetInspector))
    , mSceneAssetInspector(std::move(sceneAssetInspector))
    , mModelAssetInspector(std::move(modelAssetInspector))
    , mTextureAssetInspector(std::move(textureAssetInspector))
    , mEditorSelection(std::move(editorSelection))
  {
  }

  void
  AssetInspector::RenderBasicInspector(IAssetReference& asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0F);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0F);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0F);
    ImGui::TextWrapped("Unknown");
  }

  void
  AssetInspector::Render()
  {
    if (mEditorSelection->GetSelectedAsset() == nullptr)
    {
      return;
    }

    IAssetReference& asset = *mEditorSelection->GetSelectedAsset();
    switch (asset.GetType())
    {
      using enum ASSET_TYPE;
      case MODEL:
        {
          mModelAssetInspector->Render(asset);
          break;
        }
      case TEXTURE:
        {
          mTextureAssetInspector->Render(asset);
          break;
        }
      case SCENE:
        {
          mSceneAssetInspector->Render(asset);
          break;
        }
      case MATERIAL:
        {
          mMaterialAssetInspector->Render(asset);
          break;
        }
      case VERTEX_SHADER:
      case TESC_SHADER:
      case TESE_SHADER:
      case GEOMETRY_SHADER:
      case FRAGMENT_SHADER:
      case HLSL_SHADER:
      case COMPUTE_SHADER:
      case UNKNOWN:
      default: RenderBasicInspector(asset); break;
    }
  }
}