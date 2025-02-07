#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Base.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Editor/Modules/Inspector/AssetInspector/TextureAsset/ITextureAssetInspector.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  class TextureAssetInspector : public ITextureAssetInspector
  {
  private:
    GraphicsApi                       m_GraphicsApi;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IAssetReimporter> m_AssetReimporter;
    std::shared_ptr<IInputManager>    m_InputManager;

  public:
    TextureAssetInspector(GraphicsApi                       graphicsApi,
                          std::shared_ptr<IAssetDatabase>   assetDatabase,
                          std::shared_ptr<IAssetReimporter> assetReimporter,
                          std::shared_ptr<IMaterialPreview> materialPreview,
                          std::shared_ptr<IInputManager>    inputManager);
    ~TextureAssetInspector() override = default;

    void
    Render(IAssetReference& asset) override;
  };
}