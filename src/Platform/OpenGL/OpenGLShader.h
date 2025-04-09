#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Logging/IDwarfLogger.h"
#include <boost/di.hpp>
#include <boost/serialization/strong_typedef.hpp>
#include <cstdint>
#include <fmt/format.h>
#include <glad/glad.h>

namespace Dwarf
{
  struct ShaderLogs
  {
    std::string mVertexShaderLog;
    std::string mTessellationControlShaderLog;
    std::string mTessellationEvaluationShaderLog;
    std::string mGeometryShaderLog;
    std::string mFragmentShaderLog;
  };

  class OpenGLShader : public IShader
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IVramTracker> mVramTracker;
    std::shared_ptr<IShaderParameterCollectionFactory>
               mShaderParameterCollectionFactory;
    GLuint     mID = -1;
    int        mNextTextureSlot = 0;
    ShaderLogs mShaderLogs;
    bool       mSuccessfullyCompiled = false;

    std::map<std::string, GLint>                mUniformLocations;
    std::map<std::string, ShaderParameterValue> mUniformStates;
    std::map<std::string, ShaderParameterValue> mUniformStatesDraft;
    std::map<int, uintptr_t>                    mTextureStates;
    std::map<int, uintptr_t>                    mTextureStatesDraft;

    std::optional<std::unique_ptr<IAssetReference>> mVertexShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>> mGeometryShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>>
      mTessellationControlShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>>
      mTessellationEvaluationShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>> mFragmentShaderAsset;

    struct HandleShaderSourceVisitor;
    friend struct HandleShaderSourceVisitor;

  public:
    BOOST_DI_INJECT(OpenGLShader,
                    std::unique_ptr<IShaderSourceCollection> shaderSources,
                    std::shared_ptr<IShaderParameterCollectionFactory>
                      shaderParameterCollectionFactory,
                    std::shared_ptr<IDwarfLogger> logger,
                    std::shared_ptr<IVramTracker> vramTracker);
    ~OpenGLShader() override;

    [[nodiscard]] auto
    GetID() const -> GLuint;

    void
    ResetUniformBindings();

    void
    UploadParameters();

    /**
     * @brief Compiles the shader program
     *
     */
    void
    Compile() override;

    /**
     * @brief Returns the compilation status of the shader.
     *
     * @return true if the shader has been compiled, false otherwise.
     */
    [[nodiscard]] auto
    IsCompiled() const -> bool override;

    /**
     * @brief Creates a ShaderParameterCollection that contains all the shader
     * parameters that the shader uses
     *
     * @return Unique pointer to the created ShaderParameterCollection
     */
    auto
    CreateParameters() -> std::unique_ptr<IShaderParameterCollection> override;

    [[nodiscard]] auto
    GetShaderLogs() const -> const ShaderLogs&;

    void
    SetParameter(std::string          identifier,
                 ShaderParameterValue parameter) override;

    auto
    GetUniformLocation(std::string uniformName) -> GLint;

    static const std::array<std::string, 5> ReservedUniformNames;

    [[nodiscard]] auto
    CompareTo(const IShader& other) const -> bool;

    auto
    operator<(const IShader& other) const -> bool override
    {
      return CompareTo(other);
    }
  };
}