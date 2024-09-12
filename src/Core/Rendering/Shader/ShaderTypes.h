#pragma once
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(std::string, ComputeShaderSource);
  using ShaderSource = std::variant<
    std::unique_ptr<IAssetReference<VertexShaderAsset>>,
    std::unique_ptr<IAssetReference<FragmentShaderAsset>>,
    std::unique_ptr<IAssetReference<GeometryShaderAsset>>,
    std::unique_ptr<IAssetReference<TessellationControlShaderAsset>>,
    std::unique_ptr<IAssetReference<TessellationEvaluationShaderAsset>>>;
  using ShaderSourceCollection = std::vector<ShaderSource>;
  // BOOST_STRONG_TYPEDEF(ShaderSourcesAlias, ShaderSourceCollection);
}