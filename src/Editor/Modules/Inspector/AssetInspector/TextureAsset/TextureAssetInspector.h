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
    GraphicsApi                       m_GraphicsApi;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IAssetReimporter> m_AssetReimporter;
    std::shared_ptr<IInputManager>    m_InputManager;
    std::shared_ptr<IAssetMetadata>   m_AssetMetadata;
    std::filesystem::path             m_CurrentTexturePath;
    nlohmann::json                    m_CurrentMetadata;
    TextureImportSettings             m_CurrentImportSettings;

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