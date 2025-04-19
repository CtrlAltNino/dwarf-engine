#pragma once

#include "Core/Asset/AssetReference/IAssetReference.hpp"
#include "Core/Rendering/Shader/IComputeShader.hpp"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.hpp"
#include "Core/Rendering/VramTracker/IVramTracker.hpp"
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
    bool mSuccessfullyCompiled = false;
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

    [[nodiscard]] auto
    GetID() const -> GLuint;

    void
    Compile() override;

    [[nodiscard]] auto
    IsCompiled() const -> bool override;

    [[nodiscard]] auto
    GetLog() const -> const std::string&;

    auto
    GetParameters() -> std::shared_ptr<IShaderParameterCollection> override;
  };
}