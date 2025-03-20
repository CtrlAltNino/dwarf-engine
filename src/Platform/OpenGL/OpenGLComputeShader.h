#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/Shader/IComputeShader.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Core/Rendering/Shader/ShaderTypes.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include <boost/di.hpp>
#include <glad/glad.h>
#include <memory>

namespace Dwarf
{
  class OpenGLComputeShader : public IComputeShader
  {
  private:
    GLuint      mID = -1;
    std::string mComputeShaderLog;
    // Flag to determine if the shader has been successfully compiled.
    bool mSuccessfullyCompiled;
    std::shared_ptr<IShaderParameterCollectionFactory>
                                     mShaderParameterCollectionFactory;
    std::unique_ptr<IAssetReference> mComputeShaderAsset;
    std::shared_ptr<IVramTracker>    mVramTracker;

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