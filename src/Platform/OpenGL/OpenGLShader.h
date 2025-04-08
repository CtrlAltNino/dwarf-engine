#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Logging/IDwarfLogger.h"
#include "Platform/OpenGL/OpenGLTexture.h"
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
    GLuint     mID = -1;
    ShaderLogs mShaderLogs;
    // Flag to determine if the shader has been successfully compiled.
    bool mSuccessfullyCompiled = false;
    // Map of parameters that the shader uses.
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IVramTracker> mVramTracker;
    std::shared_ptr<IShaderParameterCollectionFactory>
      mShaderParameterCollectionFactory;

    std::map<std::string, GLint> mUniformLocations;
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
               uint32_t      textureCount);
    void
    SetUniform(std::string    uniformName,
               OpenGLTexture& value,
               uint32_t       textureCount);
    void
    SetUniform(std::string uniformName, glm::vec2 value);
    void
    SetUniform(std::string uniformName, glm::vec3 value);
    void
    SetUniform(std::string uniformName, glm::vec4 value);
    void
    SetUniform(std::string uniformName, glm::mat4 value);

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