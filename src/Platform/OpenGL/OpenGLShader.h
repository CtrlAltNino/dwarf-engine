#pragma once
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "pch.h"

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Logging/IDwarfLogger.h"

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Rendering/Shader/IShader.h"
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
    GLuint
    GetID() const;

    void
    Compile() override;

    bool
    IsCompiled() const override;

    std::unique_ptr<IShaderParameterCollection>
    CreateParameters() override;

    const ShaderLogs&
    GetShaderLogs() const;

    std::optional<std::unique_ptr<IAssetReference>>&
    GetVertexShaderAsset();

    std::optional<std::unique_ptr<IAssetReference>>&
    GetFragmentShaderAsset();

    std::optional<std::unique_ptr<IAssetReference>>&
    GetGeometryShaderAsset();

    std::optional<std::unique_ptr<IAssetReference>>&
    GetTessellationControlShaderAsset();

    std::optional<std::unique_ptr<IAssetReference>>&
    GetTessellationEvaluationShaderAsset();

    void
    SetUniform(std::string uniformName, bool value);
    void
    SetUniform(std::string uniformName, int value);
    void
    SetUniform(std::string uniformName, uint32_t value);
    void
    SetUniform(std::string uniformName, float value);
    void
    SetUniform(std::string   uniformName,
               TextureAsset& value,
               unsigned int  textureCount);
    void
    SetUniform(std::string uniformName, glm::vec2 value);
    void
    SetUniform(std::string uniformName, glm::vec3 value);
    void
    SetUniform(std::string uniformName, glm::vec4 value);
    void
    SetUniform(std::string uniformName, glm::mat4 value);

    GLuint
    GetUniformLocation(std::string uniformName);

    static const std::array<std::string, 5> ReservedUniformNames;

    nlohmann::json
    Serialize() override;

    bool
    CompareTo(const IShader& other) const;

    bool
    operator<(const IShader& other) const override
    {
      return CompareTo(other);
    }

  private:
    GLuint     mID = -1;
    ShaderLogs mShaderLogs;
    // Flag to determine if the shader has been successfully compiled.
    bool mSuccessfullyCompiled;
    // Map of parameters that the shader uses.
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IVramTracker> mVramTracker;
    std::shared_ptr<IShaderParameterCollectionFactory>
      mShaderParameterCollectionFactory;

    std::map<std::string, GLuint> mUniformLocations;
    std::map<std::string,
             std::variant<bool,
                          int,
                          uint32_t,
                          float,
                          glm::vec2,
                          glm::vec3,
                          glm::vec4,
                          glm::mat3,
                          glm::mat4>>
      mUniformStates;

    std::map<int, uintptr_t> mTextureStates;

    std::optional<std::unique_ptr<IAssetReference>> mVertexShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>> mGeometryShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>>
      mTessellationControlShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>>
      mTessellationEvaluationShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>> mFragmentShaderAsset;
  };
}