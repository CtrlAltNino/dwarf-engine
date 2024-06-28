#pragma once

#include "Core/Rendering/Shader/IComputeShader.h"
#include "Core/Rendering/Shader/ShaderTypes.h"
#include <glad/glad.h>
#include <memory>

namespace Dwarf
{
  class OpenGLComputeShader : public IComputeShader
  {
  private:
    GLuint      m_ID = -1;
    std::string m_ComputeShaderLog;
    std::string m_Source;
    // Flag to determine if the shader has been successfully compiled.
    bool m_SuccessfullyCompiled;
    // Map of parameters that the shader uses.
    std::shared_ptr<IShaderParameterCollection> m_Parameters;

  public:
    OpenGLComputeShader(const ComputeShaderSource&                  source,
                        std::shared_ptr<IShaderParameterCollection> parameters);
    ~OpenGLComputeShader() override = default;

    GLuint
    GetID() const;

    void
    Compile() override;

    bool
    IsCompiled() const override;

    const std::string&
    GetLog() const;

    void
    GenerateParameters();

    std::shared_ptr<IShaderParameterCollection>
    GetParameters() override;
  };
}