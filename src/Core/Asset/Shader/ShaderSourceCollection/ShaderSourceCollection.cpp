#include "ShaderSourceCollection.h"

namespace Dwarf
{
  ShaderSourceCollection::ShaderSourceCollection(
    std::vector<ShaderSource>& shaderSources)
    : m_ShaderSources(std::move(shaderSources))
  {
  }

  std::vector<ShaderSource>&
  ShaderSourceCollection::GetShaderSources()
  {
    return m_ShaderSources;
  }
} // namespace Dwarf