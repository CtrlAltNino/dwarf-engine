#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Base.h"
#include "Core/Rendering/PreviewRenderer/ModelPreview/IModelPreview.h"
#include "Editor/Modules/Inspector/AssetInspector/ModelAsset/IModelAssetInspector.h"
#include "Editor/Stats/IEditorStats.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  class ModelAssetInspector : public IModelAssetInspector
  {
  private:
    GraphicsApi                       mGraphicsApi;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    std::shared_ptr<IAssetReimporter> mAssetReimporter;
    std::shared_ptr<IModelPreview>    mModelPreview;
    std::shared_ptr<IInputManager>    mInputManager;
    std::shared_ptr<IEditorStats>     mEditorStats;

  public:
    ModelAssetInspector(GraphicsApi                       graphicsApi,
                        std::shared_ptr<IAssetDatabase>   assetDatabase,
                        std::shared_ptr<IAssetReimporter> assetReimporter,
                        std::shared_ptr<IModelPreview>    modelPreview,
                        std::shared_ptr<IInputManager>    inputManager,
                        std::shared_ptr<IEditorStats>     editorStats);
    ~ModelAssetInspector() override = default;

    void
    Render(IAssetReference& asset) override;
  };
}