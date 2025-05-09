#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.hpp"
#include "Core/Asset/Database/AssetComponents.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Rendering/Material/IO/IMaterialIO.hpp"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.hpp"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.hpp"
#include "Editor/Modules/Inspector/AssetInspector/MaterialAsset/IMaterialAssetInspector.hpp"
#include "Input/IInputManager.hpp"
#include "ShaderAssetSelector/IShaderAssetSelector.hpp"
#include "ShaderAssetSelector/IShaderAssetSelectorFactory.hpp"
#include <optional>

namespace Dwarf
{
  class MaterialAssetInspector : public IMaterialAssetInspector
  {
  private:
    GraphicsApi                                          mGraphicsApi;
    std::shared_ptr<IAssetDatabase>                      mAssetDatabase;
    std::shared_ptr<IAssetReimporter>                    mAssetReimporter;
    std::shared_ptr<IMaterialPreview>                    mMaterialPreview;
    std::shared_ptr<IInputManager>                       mInputManager;
    std::shared_ptr<IMaterialIO>                         mMaterialIO;
    std::shared_ptr<IShaderRegistry>                     mShaderRegistry;
    std::shared_ptr<IShaderAssetSelector>                mShaderAssetSelector;
    std::optional<std::reference_wrapper<MaterialAsset>> mCurrentAsset =
      std::nullopt;
    std::filesystem::path mAssetPath;
    IMaterial*            mMaterialAssetMemory;

    void
    RenderAssetInfo();

    void
    RenderMaterialProperties();

    void
    RenderMaterialShader();

    void
    RenderShaderAssetDropdowns();

    void
    RenderShaderParameters();

    void
    RenderMaterialButtons();

    auto
    RenderMaterialPreview() -> float;

  public:
    MaterialAssetInspector(GraphicsApi                       graphicsApi,
                           std::shared_ptr<IAssetDatabase>   assetDatabase,
                           std::shared_ptr<IAssetReimporter> assetReimporter,
                           std::shared_ptr<IMaterialPreview> materialPreview,
                           std::shared_ptr<IInputManager>    inputManager,
                           std::shared_ptr<IMaterialIO>      materialIO,
                           std::shared_ptr<IShaderRegistry>  shaderRegistry,
                           const std::shared_ptr<IShaderAssetSelectorFactory>&
                             shaderAssetSelectorFactory);
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