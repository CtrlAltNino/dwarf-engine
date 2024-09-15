#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
#include "IShader.h"

namespace Dwarf
{
  class IShaderFactory
  {
  public:
    virtual ~IShaderFactory() = default;

    virtual std::unique_ptr<IShader>
    CreateDefaultShader() = 0;

    virtual std::unique_ptr<IShader>
    CreateShader(std::unique_ptr<IShaderSourceCollection> shaderSources) = 0;

    virtual std::unique_ptr<IShader>
    CreateShader(const nlohmann::json& serializedShader) = 0;
  };
} // namespace Dwarf