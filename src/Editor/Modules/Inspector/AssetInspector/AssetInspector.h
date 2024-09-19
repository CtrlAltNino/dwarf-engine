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
#include "Editor/Stats/IEditorStats.h"
#include "Input/IInputManager.h"
#include <memory>

namespace Dwarf
{
  class AssetInspector : public IAssetInspector
  {
  public:
    // GraphicsApi                       m_GraphicsApi;
    // std::shared_ptr<ILoadedScene>     m_LoadedScene;
    //  std::shared_ptr<ISceneIO>         m_SceneIO;
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;
    // std::shared_ptr<IMaterialPreview> m_MaterialPreview;
    // std::shared_ptr<IModelPreview>    m_ModelPreview;
    // std::shared_ptr<IInputManager>    m_InputManager;
    // std::shared_ptr<IEditorStats>     m_EditorStats;
    // std::shared_ptr<IAssetReimporter> m_AssetReimporter;
    std::shared_ptr<IMaterialAssetInspector> m_MaterialAssetInspector;
    std::shared_ptr<ISceneAssetInspector>    m_SceneAssetInspector;
    std::shared_ptr<IModelAssetInspector>    m_ModelAssetInspector;
    std::shared_ptr<ITextureAssetInspector>  m_TextureAssetInspector;

    AssetInspector(
      std::shared_ptr<IAssetDatabase>          assetDatabase,
      std::shared_ptr<IMaterialAssetInspector> materialAssetInspector,
      std::shared_ptr<ISceneAssetInspector>    sceneAssetInspector,
      std::shared_ptr<IModelAssetInspector>    modelAssetInspector,
      std::shared_ptr<ITextureAssetInspector>  textureAssetInspector);

    ~AssetInspector() override = default;
    void
    Render(const std::filesystem::path& assetPath) override;

  private:
    void
    RenderBasicInspector(IAssetReference& asset);
  };
}