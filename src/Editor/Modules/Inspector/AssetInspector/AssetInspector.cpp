#include "pch.h"
#include "AssetInspector.h"
#include <string>

namespace Dwarf
{
  AssetInspector::AssetInspector(
    std::shared_ptr<IAssetDatabase>          assetDatabase,
    std::shared_ptr<IMaterialAssetInspector> materialAssetInspector,
    std::shared_ptr<ISceneAssetInspector>    sceneAssetInspector,
    std::shared_ptr<IModelAssetInspector>    modelAssetInspector,
    std::shared_ptr<ITextureAssetInspector>  textureAssetInspector,
    std::shared_ptr<IEditorSelection>        editorSelection)
    : m_AssetDatabase(assetDatabase)
    , m_MaterialAssetInspector(materialAssetInspector)
    , m_SceneAssetInspector(sceneAssetInspector)
    , m_ModelAssetInspector(modelAssetInspector)
    , m_TextureAssetInspector(textureAssetInspector)
    , m_EditorSelection(editorSelection)
  {
  }

  void
  AssetInspector::RenderBasicInspector(IAssetReference& asset)
  {
    ImGui::TextWrapped("File name: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().filename().string().c_str());

    ImGui::TextWrapped("Path: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("%s", asset.GetPath().string().c_str());

    ImGui::TextWrapped("Type: ");
    ImGui::SameLine(0, 5.0f);
    ImGui::TextWrapped("Unknown");
  }

  void
  AssetInspector::Render()
  {
    IAssetReference& asset = m_EditorSelection->GetSelectedAsset();
    switch (asset.GetType())
    {
      using enum ASSET_TYPE;
      case MODEL:
        {
          m_ModelAssetInspector->Render(asset);
          break;
        }
      case TEXTURE:
        {
          m_TextureAssetInspector->Render(asset);
          break;
        }
      case SCENE:
        {
          m_SceneAssetInspector->Render(asset);
          break;
        }
      case MATERIAL:
        {
          m_MaterialAssetInspector->Render(asset);
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