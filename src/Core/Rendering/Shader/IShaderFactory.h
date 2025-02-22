#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
#include "IShader.h"

namespace Dwarf
{
  class IShaderFactory
  {
  public:
    virtual ~IShaderFactory() = default;

    virtual std::shared_ptr<IShader>
    CreateDefaultShader() = 0;

    virtual std::shared_ptr<IShader>
    CreateErrorShader() = 0;

    virtual std::shared_ptr<IShader>
    CreateShader(std::unique_ptr<IShaderSourceCollection> shaderSources) = 0;
  };
} // namespace Dwarf