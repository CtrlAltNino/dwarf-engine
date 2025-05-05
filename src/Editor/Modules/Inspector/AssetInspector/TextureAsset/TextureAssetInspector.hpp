#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Asset/Metadata/IAssetMetadata.hpp"
#include "Core/Base.hpp"
#include "Core/Rendering/RendererApi/IRendererApi.hpp"
#include "Core/Rendering/RendererApi/IRendererApiFactory.hpp"
#include "Editor/Modules/Inspector/AssetInspector/TextureAsset/ITextureAssetInspector.hpp"
#include "Input/IInputManager.hpp"
#include "Utilities/ImageUtilities/TextureCommon.hpp"

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
    std::shared_ptr<IRendererApi>     mRendererApi;
    std::filesystem::path             mCurrentTexturePath;
    nlohmann::json                    mCurrentMetadata;
    TextureImportSettings             mCurrentImportSettings;

  public:
    TextureAssetInspector(
      GraphicsApi                                 graphicsApi,
      std::shared_ptr<IAssetDatabase>             assetDatabase,
      std::shared_ptr<IAssetReimporter>           assetReimporter,
      std::shared_ptr<IInputManager>              inputManager,
      std::shared_ptr<IAssetMetadata>             assetMetadata,
      const std::shared_ptr<IRendererApiFactory>& rendererApiFactory);
    ~TextureAssetInspector() override = default;

    /**
     * @brief Renders the texture asset inspector
     *
     * @param asset
     */
    void
    Render(IAssetReference& asset) override;
  };
}