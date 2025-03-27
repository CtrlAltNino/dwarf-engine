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
      std::shared_ptr<IAssetDatabase> assetDatabase,
      std::shared_ptr<IShaderSourceCollectionFactory>
                                               shaderSourceCollectionFactory,
      std::unique_ptr<IShaderSourceCollection> shaderSources);

    virtual ~OpenGLShaderAssetSourceContainer() = default;

    auto
    GetShaderSources() -> std::unique_ptr<IShaderSourceCollection> override;

    auto
    GetVertexShaderAsset() -> std::optional<std::unique_ptr<IAssetReference>>&;

    auto
    GetFragmentShaderAsset()
      -> std::optional<std::unique_ptr<IAssetReference>>&;

    auto
    GetGeometryShaderAsset()
      -> std::optional<std::unique_ptr<IAssetReference>>&;

    auto
    GetTessellationControlShaderAsset()
      -> std::optional<std::unique_ptr<IAssetReference>>&;

    auto
    GetTessellationEvaluationShaderAsset()
      -> std::optional<std::unique_ptr<IAssetReference>>&;
  };
}