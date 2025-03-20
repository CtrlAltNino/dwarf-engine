#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Base.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Core/Rendering/PreviewRenderer/ModelPreview/IModelPreview.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Modules/Inspector/AssetInspector/IAssetInspector.h"
#include "Editor/Modules/Inspector/AssetInspector/MaterialAsset/IMaterialAssetInspector.h"
#include "Editor/Modules/Inspector/AssetInspector/ModelAsset/IModelAssetInspector.h"
#include "Editor/Modules/Inspector/AssetInspector/SceneAsset/ISceneAssetInspector.h"
#include "Editor/Modules/Inspector/AssetInspector/TextureAsset/ITextureAssetInspector.h"
#include "Editor/Selection/IEditorSelection.h"
#include "Editor/Stats/IEditorStats.h"
#include "Input/IInputManager.h"
#include <memory>

namespace Dwarf
{
  class AssetInspector : public IAssetInspector
  {
  public:
    // GraphicsApi                       mGraphicsApi;
    // std::shared_ptr<ILoadedScene>     mLoadedScene;
    //  std::shared_ptr<ISceneIO>         mSceneIO;
    std::shared_ptr<IAssetDatabase> mAssetDatabase;
    // std::shared_ptr<IMaterialPreview> mMaterialPreview;
    // std::shared_ptr<IModelPreview>    mModelPreview;
    // std::shared_ptr<IInputManager>    mInputManager;
    // std::shared_ptr<IEditorStats>     mEditorStats;
    // std::shared_ptr<IAssetReimporter> mAssetReimporter;
    std::shared_ptr<IMaterialAssetInspector> mMaterialAssetInspector;
    std::shared_ptr<ISceneAssetInspector>    mSceneAssetInspector;
    std::shared_ptr<IModelAssetInspector>    mModelAssetInspector;
    std::shared_ptr<ITextureAssetInspector>  mTextureAssetInspector;
    std::shared_ptr<IEditorSelection>        mEditorSelection;

    AssetInspector(
      std::shared_ptr<IAssetDatabase>          assetDatabase,
      std::shared_ptr<IMaterialAssetInspector> materialAssetInspector,
      std::shared_ptr<ISceneAssetInspector>    sceneAssetInspector,
      std::shared_ptr<IModelAssetInspector>    modelAssetInspector,
      std::shared_ptr<ITextureAssetInspector>  textureAssetInspector,
      std::shared_ptr<IEditorSelection>        editorSelection);

    ~AssetInspector() override = default;
    void
    Render() override;

  private:
    void
    RenderBasicInspector(IAssetReference& asset);
  };
}