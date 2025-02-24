#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
#include "Core/Rendering/Shader/IShader.h"
namespace Dwarf
{
  class IShaderRegistry
  {
  public:
    virtual ~IShaderRegistry() = default;
    virtual std::shared_ptr<IShader>
    GetOrCreate(std::unique_ptr<IShaderSourceCollection> shaderSources) = 0;

    virtual std::shared_ptr<IShader>
    GetOrCreate(const nlohmann::json& serializedShader) = 0;
  };
}