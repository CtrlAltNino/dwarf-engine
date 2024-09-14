#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"

namespace Dwarf
{
  class ShaderSourceCollection : public IShaderSourceCollection
  {
  private:
    std::vector<ShaderSource> m_ShaderSources;

  public:
    ShaderSourceCollection(std::vector<ShaderSource>& shaderSources);
    ~ShaderSourceCollection() override = default;
    std::vector<ShaderSource>&
    GetShaderSources() override;
  };
}