#pragma once

#include "Core/Base.h"
#include "IShader.h"

namespace Dwarf
{
  class IShaderFactory
  {
  public:
    virtual ~IShaderFactory() = default;

    virtual std::shared_ptr<IShader>
    CreateShader() = 0;

    virtual std::shared_ptr<IShader>
    CreateShader(const std::string& vertexShaderPath,
                 const std::string& fragmentShaderPath) = 0;

    virtual std::shared_ptr<IShader>
    CreateShader(const nlohmann::json& serializedShader) = 0;
  };
} // namespace Dwarf