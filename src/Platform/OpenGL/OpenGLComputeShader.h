#pragma once

#include "Core/Asset/AssetComponents.h"
#include "Core/Rendering/Shader/IComputeShader.h"
#include "Core/Rendering/Shader/ShaderTypes.h"
#include "Core/Asset/AssetReference.h"
#include <glad/glad.h>
#include <memory>

namespace Dwarf
{
  class OpenGLComputeShader : public IComputeShader
  {
  private:
    GLuint      m_ID = -1;
    std::string m_ComputeShaderLog;
    // Flag to determine if the shader has been successfully compiled.
    bool                                                m_SuccessfullyCompiled;
    std::shared_ptr<AssetReference<ComputeShaderAsset>> m_ComputeShaderAsset;

  public:
    BOOST_DI_INJECT(
      OpenGLComputeShader,
      std::shared_ptr<AssetReference<ComputeShaderAsset>> computeShaderAsset);
    ~OpenGLComputeShader() override = default;

    GLuint
    GetID() const;

    void
    Compile() override;

    bool
    IsCompiled() const override;

    const std::string&
    GetLog() const;

    std::shared_ptr<IShaderParameterCollection>
    GetParameters() override;
  };
}