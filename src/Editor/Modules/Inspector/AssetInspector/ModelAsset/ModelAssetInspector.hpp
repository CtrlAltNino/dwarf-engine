#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Base.hpp"
#include "Core/Rendering/PreviewRenderer/ModelPreview/IModelPreview.hpp"
#include "Editor/Modules/Inspector/AssetInspector/ModelAsset/IModelAssetInspector.hpp"
#include "Editor/Stats/IEditorStats.hpp"
#include "Input/IInputManager.hpp"

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

    /**
     * @brief Renders the model asset inspector
     *
     * @param asset
     */
    void
    Render(IAssetReference& asset) override;
  };
}