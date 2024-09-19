#include "ShaderSourceCollection.h"

namespace Dwarf
{
  ShaderSourceCollection::ShaderSourceCollection(
    std::vector<std::unique_ptr<IAssetReference>>& shaderSources)
    : m_ShaderSources(std::move(shaderSources))
  {
  }

  std::vector<std::unique_ptr<IAssetReference>>&
  ShaderSourceCollection::GetShaderSources()
  {
    return m_ShaderSources;
  }
} // namespace Dwarf