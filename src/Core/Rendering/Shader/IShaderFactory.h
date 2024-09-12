#pragma once

#include "Core/Base.h"
#include "Core/Rendering/Shader/ShaderTypes.h"
#include "IShader.h"

namespace Dwarf
{
  class IShaderFactory
  {
  public:
    virtual ~IShaderFactory() = default;

    virtual std::unique_ptr<IShader>
    CreateShader() = 0;

    virtual std::unique_ptr<IShader>
    CreateShader(ShaderSourceCollection& shaderSources) = 0;

    virtual std::unique_ptr<IShader>
    CreateShader(const nlohmann::json& serializedShader) = 0;
  };
} // namespace Dwarf