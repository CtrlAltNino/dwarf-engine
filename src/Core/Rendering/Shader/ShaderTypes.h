#pragma once
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/AssetReference.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(std::string, ComputeShaderSource);
  using ShaderSourcesAlias = std::vector<std::variant<
    std::shared_ptr<AssetReference<VertexShaderAsset>>,
    std::shared_ptr<AssetReference<FragmentShaderAsset>>,
    std::shared_ptr<AssetReference<GeometryShaderAsset>>,
    std::shared_ptr<AssetReference<TessellationControlShaderAsset>>,
    std::shared_ptr<AssetReference<TessellationEvaluationShaderAsset>>>>;
  BOOST_STRONG_TYPEDEF(ShaderSourcesAlias, ShaderSourceCollection);
}