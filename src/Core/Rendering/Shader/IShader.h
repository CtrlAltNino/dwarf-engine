#pragma once

#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include "Utilities/ISerializable.h"

#include <memory>

namespace Dwarf
{
  class IShader : public ISerializable
  {
  public:
    virtual ~IShader() = default;

    // Compiles the shader.
    virtual void
    Compile() = 0;

    /**
     * @brief Returns the compilation status of the shader.
     *
     * @return true if the shader has been compiled, false otherwise.
     */
    virtual bool
    IsCompiled() const = 0;

    virtual std::unique_ptr<IShaderParameterCollection>
    CreateParameters() = 0;

    virtual nlohmann::json
    Serialize() const = 0;
  };
}