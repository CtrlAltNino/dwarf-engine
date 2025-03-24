#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Editor/Modules/Inspector/AssetInspector/MaterialAsset/IMaterialAssetInspector.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  class MaterialAssetInspector : public IMaterialAssetInspector
  {
  private:
    GraphicsApi                       mGraphicsApi;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    std::shared_ptr<IAssetReimporter> mAssetReimporter;
    std::shared_ptr<IMaterialPreview> mMaterialPreview;
    std::shared_ptr<IInputManager>    mInputManager;
    std::shared_ptr<IMaterialIO>      mMaterialIO;

  public:
    MaterialAssetInspector(GraphicsApi                       graphicsApi,
                           std::shared_ptr<IAssetDatabase>   assetDatabase,
                           std::shared_ptr<IAssetReimporter> assetReimporter,
                           std::shared_ptr<IMaterialPreview> materialPreview,
                           std::shared_ptr<IInputManager>    inputManager,
                           std::shared_ptr<IMaterialIO>      materialIO);
    ~MaterialAssetInspector() override = default;

    /**
     * @brief Renders the inspector for a material asset reference
     *
     * @param asset
     */
    void
    Render(IAssetReference& asset) override;
  };
}