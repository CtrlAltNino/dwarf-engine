#include "ShaderSourceCollection.h"

namespace Dwarf
{
  ShaderSourceCollection::ShaderSourceCollection(
    std::vector<std::unique_ptr<IAssetReference>>& shaderSources)
    : mShaderSources(std::move(shaderSources))
  {
  }

  auto
  ShaderSourceCollection::GetShaderSources()
    -> std::vector<std::unique_ptr<IAssetReference>>&
  {
    return mShaderSources;
  }
} // namespace Dwarf