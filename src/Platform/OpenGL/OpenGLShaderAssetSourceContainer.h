#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/Material/ShaderAssetSourceContainer/IShaderAssetSourceContainer.h"

namespace Dwarf
{
  class OpenGLShaderAssetSourceContainer : public IShaderAssetSourceContainer
  {
  private:
    std::weak_ptr<IAssetDatabase> mAssetDatabase;
    std::weak_ptr<IShaderSourceCollectionFactory>
      mShaderSourceCollectionFactory;
    std::optional<std::unique_ptr<IAssetReference>> mVertexShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>> mGeometryShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>>
      mTessellationControlShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>>
      mTessellationEvaluationShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>> mFragmentShaderAsset;

  public:
    OpenGLShaderAssetSourceContainer(
      const std::shared_ptr<IAssetDatabase>& assetDatabase,
      const std::shared_ptr<IShaderSourceCollectionFactory>&
                                               shaderSourceCollectionFactory,
      std::unique_ptr<IShaderSourceCollection> shaderSources);

    virtual ~OpenGLShaderAssetSourceContainer() = default;

    /**
     * @brief Gets the stores assets as a ShaderSourceCollection
     *
     * @return Unique pointer to the created ShaderSourceCollection instance
     */
    auto
    GetShaderSources() -> std::unique_ptr<IShaderSourceCollection> override;

    /**
     * @brief Gets the AssetReference to the stored vertex shader if there is
     * one
     *
     * @return Reference to the AssetReference
     */
    auto
    GetVertexShaderAsset() -> std::optional<std::unique_ptr<IAssetReference>>&;

    /**
     * @brief Gets the AssetReference to the stored fragment shader if there is
     * one
     *
     * @return Reference to the AssetReference
     */
    auto
    GetFragmentShaderAsset()
      -> std::optional<std::unique_ptr<IAssetReference>>&;

    /**
     * @brief Gets the AssetReference to the stored geometry shader if there is
     * one
     *
     * @return Reference to the AssetReference
     */
    auto
    GetGeometryShaderAsset()
      -> std::optional<std::unique_ptr<IAssetReference>>&;

    /**
     * @brief Gets the AssetReference to the stored tessellation control shader
     * if there is one
     *
     * @return Reference to the AssetReference
     */
    auto
    GetTessellationControlShaderAsset()
      -> std::optional<std::unique_ptr<IAssetReference>>&;

    /**
     * @brief Gets the AssetReference to the stored tessellation evaluation
     * shader if there is one
     *
     * @return Reference to the AssetReference
     */
    auto
    GetTessellationEvaluationShaderAsset()
      -> std::optional<std::unique_ptr<IAssetReference>>&;

    auto
    Serialize() -> nlohmann::json override;
  };
}