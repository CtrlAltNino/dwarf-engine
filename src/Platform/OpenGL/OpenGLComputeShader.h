#pragma once

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Rendering/Shader/IComputeShader.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Core/Rendering/Shader/ShaderTypes.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include <glad/glad.h>
#include <memory>
#include <boost/di.hpp>

namespace Dwarf
{
  class OpenGLComputeShader : public IComputeShader
  {
  private:
    GLuint      m_ID = -1;
    std::string m_ComputeShaderLog;
    // Flag to determine if the shader has been successfully compiled.
    bool m_SuccessfullyCompiled;
    std::shared_ptr<IShaderParameterCollectionFactory>
                                     m_ShaderParameterCollectionFactory;
    std::unique_ptr<IAssetReference> m_ComputeShaderAsset;
    std::shared_ptr<IVramTracker>    m_VramTracker;

  public:
    BOOST_DI_INJECT(OpenGLComputeShader,
                    std::unique_ptr<IAssetReference>& computeShaderAsset,
                    std::shared_ptr<IShaderParameterCollectionFactory>
                      shaderParameterCollectionFactory,
                    std::shared_ptr<IVramTracker> vramTracker);
    ~OpenGLComputeShader() override;

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