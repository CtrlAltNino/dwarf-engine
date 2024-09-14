#pragma once
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(std::string, ComputeShaderSource);
  // BOOST_STRONG_TYPEDEF(ShaderSourcesAlias, ShaderSourceCollection);
}