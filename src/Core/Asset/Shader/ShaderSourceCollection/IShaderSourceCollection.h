#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"

namespace Dwarf
{
  using ShaderSource =
    std::variant<IAssetReference<VertexShaderAsset>,
                 IAssetReference<FragmentShaderAsset>,
                 IAssetReference<GeometryShaderAsset>,
                 IAssetReference<TessellationControlShaderAsset>,
                 IAssetReference<TessellationEvaluationShaderAsset>>;

  class IShaderSourceCollection
  {
  public:
    virtual ~IShaderSourceCollection() = default;

    virtual std::vector<ShaderSource>&
    GetShaderSources() = 0;
  };
}