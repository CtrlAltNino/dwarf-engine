#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Base.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Editor/Modules/Inspector/AssetInspector/TextureAsset/ITextureAssetInspector.h"
#include "Input/IInputManager.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <nlohmann/json_fwd.hpp>

namespace Dwarf
{
  class TextureAssetInspector : public ITextureAssetInspector
  {
  private:
    GraphicsApi                       mGraphicsApi;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    std::shared_ptr<IAssetReimporter> mAssetReimporter;
    std::shared_ptr<IInputManager>    mInputManager;
    std::shared_ptr<IAssetMetadata>   mAssetMetadata;
    std::filesystem::path             mCurrentTexturePath;
    nlohmann::json                    mCurrentMetadata;
    TextureImportSettings             mCurrentImportSettings;

  public:
    TextureAssetInspector(GraphicsApi                       graphicsApi,
                          std::shared_ptr<IAssetDatabase>   assetDatabase,
                          std::shared_ptr<IAssetReimporter> assetReimporter,
                          std::shared_ptr<IMaterialPreview> materialPreview,
                          std::shared_ptr<IInputManager>    inputManager,
                          std::shared_ptr<IAssetMetadata>   assetMetadata);
    ~TextureAssetInspector() override = default;

    void
    Render(IAssetReference& asset) override;
  };
}