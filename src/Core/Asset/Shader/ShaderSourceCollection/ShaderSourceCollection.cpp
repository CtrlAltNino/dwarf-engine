#include "ShaderSourceCollection.h"

namespace Dwarf
{
  ShaderSourceCollection::ShaderSourceCollection(
    std::vector<std::unique_ptr<IAssetReference>>& shaderSources)
    : mShaderSources(std::move(shaderSources))
  {
  }

  std::vector<std::unique_ptr<IAssetReference>>&
  ShaderSourceCollection::GetShaderSources()
  {
    return mShaderSources;
  }
} // namespace Dwarf