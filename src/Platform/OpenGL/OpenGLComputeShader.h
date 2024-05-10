#pragma once

#include <glad/glad.h>
#include "Core/Rendering/ComputeShader.h"
#include <Core/Rendering/IShaderParameter.h>

namespace Dwarf
{
  class OpenGLComputeShader : public ComputeShader
  {
  private:
    GLuint               m_ID = -1;
    std::string          m_ComputeShaderLog;
    std::string          m_ComputeShaderSource;
    std::shared_ptr<UID> m_ComputeShaderAsset;

  public:
    OpenGLComputeShader();
    ~OpenGLComputeShader() override;

    void
    SetComputeShader(std::shared_ptr<UID> computeShader);
    void
    SetComputeShader(std::string_view computeShader);
    std::map<std::string, std::shared_ptr<IShaderParameter>, std::less<>>
    GetParameters() override;
    void
    UpdateParameters() override;

    std::shared_ptr<UID>
    GetComputeShader() const;

    GLuint
    GetID() const;

    void
    Compile() override;

    static std::shared_ptr<OpenGLComputeShader>
    CreatePropagationShader();
    static std::shared_ptr<OpenGLComputeShader>
    CreateFinalizationShader();

    const std::string&
    GetLog() const;
    std::shared_ptr<UID>&
    GetAsset();
  };
}