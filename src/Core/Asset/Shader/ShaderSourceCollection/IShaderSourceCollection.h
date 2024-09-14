#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"

namespace Dwarf
{
  using ShaderSource = std::variant<
    std::unique_ptr<IAssetReference<VertexShaderAsset>>,
    std::unique_ptr<IAssetReference<FragmentShaderAsset>>,
    std::unique_ptr<IAssetReference<GeometryShaderAsset>>,
    std::unique_ptr<IAssetReference<TessellationControlShaderAsset>>,
    std::unique_ptr<IAssetReference<TessellationEvaluationShaderAsset>>>;

  class IShaderSourceCollection
  {
  public:
    virtual ~IShaderSourceCollection() = default;

    virtual std::vector<ShaderSource>&
    GetShaderSources() = 0;
  };
}