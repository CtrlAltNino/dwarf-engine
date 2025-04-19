#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Editor/Modules/Inspector/AssetInspector/IAssetInspector.hpp"
#include "Editor/Modules/Inspector/AssetInspector/MaterialAsset/IMaterialAssetInspector.hpp"
#include "Editor/Modules/Inspector/AssetInspector/ModelAsset/IModelAssetInspector.hpp"
#include "Editor/Modules/Inspector/AssetInspector/SceneAsset/ISceneAssetInspector.hpp"
#include "Editor/Modules/Inspector/AssetInspector/TextureAsset/ITextureAssetInspector.hpp"
#include "Editor/Selection/IEditorSelection.hpp"

namespace Dwarf
{
  class AssetInspector : public IAssetInspector
  {
  private:
    std::shared_ptr<IAssetDatabase>          mAssetDatabase;
    std::shared_ptr<IMaterialAssetInspector> mMaterialAssetInspector;
    std::shared_ptr<ISceneAssetInspector>    mSceneAssetInspector;
    std::shared_ptr<IModelAssetInspector>    mModelAssetInspector;
    std::shared_ptr<ITextureAssetInspector>  mTextureAssetInspector;
    std::shared_ptr<IEditorSelection>        mEditorSelection;

    void
    RenderBasicInspector(IAssetReference& asset);

  public:
    AssetInspector(
      std::shared_ptr<IAssetDatabase>          assetDatabase,
      std::shared_ptr<IMaterialAssetInspector> materialAssetInspector,
      std::shared_ptr<ISceneAssetInspector>    sceneAssetInspector,
      std::shared_ptr<IModelAssetInspector>    modelAssetInspector,
      std::shared_ptr<ITextureAssetInspector>  textureAssetInspector,
      std::shared_ptr<IEditorSelection>        editorSelection);

    ~AssetInspector() override = default;

    /**
     * @brief Renders the Inspector for assets
     *
     */
    void
    Render() override;
  };
}