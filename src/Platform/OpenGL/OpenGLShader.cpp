#include "OpenGLShader.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Base.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "OpenGLUtilities.h"
#include "Platform/OpenGL/OpenGLUtilities.h"
#include <algorithm>
#include <fmt/format.h>
#include <memory>
#include <utility>

#define GL_SHADER_LOG_LENGTH (1024)

namespace Dwarf
{
  OpenGLShader::OpenGLShader(
    std::unique_ptr<IShaderSourceCollection> shaderSources,
    std::shared_ptr<IShaderParameterCollectionFactory>
                                  shaderParameterCollectionFactory,
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IVramTracker> vramTracker)
    : mShaderParameterCollectionFactory(
        std::move(shaderParameterCollectionFactory))
    , mLogger(std::move(logger))
    , mVramTracker(std::move(vramTracker))
  {
    for (std::unique_ptr<IAssetReference>& shaderSource :
         shaderSources->GetShaderSources())
    {
      // std::visit(HandleShaderSourceVisitor(*this), shaderSource);
      switch (shaderSource->GetType())
      {
        case Dwarf::ASSET_TYPE::VERTEX_SHADER:
          mVertexShaderAsset = std::move(shaderSource);
          break;
        case Dwarf::ASSET_TYPE::FRAGMENT_SHADER:
          mFragmentShaderAsset = std::move(shaderSource);
          break;
        case Dwarf::ASSET_TYPE::GEOMETRY_SHADER:
          mGeometryShaderAsset = std::move(shaderSource);
          break;
        case Dwarf::ASSET_TYPE::TESC_SHADER:
          mTessellationControlShaderAsset = std::move(shaderSource);
          break;
        case Dwarf::ASSET_TYPE::TESE_SHADER:
          mTessellationEvaluationShaderAsset = std::move(shaderSource);
          break;
        default: break;
      }
    }
  }

  OpenGLShader::~OpenGLShader()
  {
    GLint binaryLength = 0;
    glGetProgramiv(mID, GL_PROGRAM_BINARY_LENGTH, &binaryLength);
    mVramTracker->RemoveShaderMemory(binaryLength);
    OpenGLUtilities::CheckOpenGLError(
      "Errors before deleting shader", "OpenGLShader", mLogger);
    glDeleteProgram(mID);
    OpenGLUtilities::CheckOpenGLError(
      "glDeleteProgram", "OpenGLShader", mLogger);
  }

  const std::array<std::string, 5> OpenGLShader::ReservedUniformNames = {
    "modelMatrix",
    "viewMatrix",
    "projectionMatrix",
    "_Time",
    "viewPosition"
  };

  const std::map<GLenum, ShaderParameterType> glTypeToDwarfShaderType = {
    { GL_BOOL, ShaderParameterType::BOOLEAN },
    { GL_INT, ShaderParameterType::INTEGER },
    { GL_UNSIGNED_INT, ShaderParameterType::UNSIGNED_INTEGER },
    { GL_FLOAT, ShaderParameterType::FLOAT },
    { GL_FLOAT_VEC2, ShaderParameterType::VEC2 },
    { GL_FLOAT_VEC3, ShaderParameterType::VEC3 },
    { GL_FLOAT_VEC4, ShaderParameterType::VEC4 },
    { GL_SAMPLER_2D, ShaderParameterType::TEX2D }
  };

  void
  OpenGLShader::Compile()
  {
    mSuccessfullyCompiled = false;

    if (!mVertexShaderAsset.has_value() || !mFragmentShaderAsset.has_value())
    {
      return;
    }

    auto& vertexShaderAsset =
      dynamic_cast<VertexShaderAsset&>(mVertexShaderAsset.value()->GetAsset());

    auto& fragmentShaderAsset = dynamic_cast<FragmentShaderAsset&>(
      mFragmentShaderAsset.value()->GetAsset());

    if (vertexShaderAsset.GetFileContent().length() > 0 &&
        fragmentShaderAsset.GetFileContent().length() > 0)
    {
      const char* vertexSource = vertexShaderAsset.GetFileContent().c_str();
      const char* fragmentSource = fragmentShaderAsset.GetFileContent().c_str();

      GLsizei vertLogLength = 0;
      GLchar  vertMessage[1024] = "";

      GLsizei fragLogLength = 0;
      GLchar  fragMessage[1024] = "";

      OpenGLUtilities::CheckOpenGLError(
        "Errors before compiling shader", "OpenGLShader", mLogger);
      GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
      OpenGLUtilities::CheckOpenGLError(
        "glCreateShader vertex shader", "OpenGLShader", mLogger);
      glShaderSource(vertexShader, 1, &vertexSource, nullptr);
      OpenGLUtilities::CheckOpenGLError(
        "glShaderSource vertex shader", "OpenGLShader", mLogger);
      glCompileShader(vertexShader);
      OpenGLUtilities::CheckOpenGLError(
        "glCompileShader vertex shader", "OpenGLShader", mLogger);

      GLint vertexCompiled = 0;
      glGetShaderInfoLog(vertexShader, 1024, &vertLogLength, vertMessage);
      OpenGLUtilities::CheckOpenGLError(
        "glGetShaderInfoLog vertex shader", "OpenGLShader", mLogger);
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);
      OpenGLUtilities::CheckOpenGLError(
        "glGetShaderiv vertex shader", "OpenGLShader", mLogger);

      if (vertLogLength > 0)
      {
        mShaderLogs.mVertexShaderLog = std::string(vertMessage);
      }

      if (vertexCompiled != GL_TRUE)
      {
        glDeleteShader(vertexShader);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteShader vertex shader", "OpenGLShader", mLogger);
        return;
      }

      mLogger->LogDebug(
        Log("Vertex shader compiled successfully", "OpenGLShader"));

      GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      OpenGLUtilities::CheckOpenGLError(
        "glCreateShader fragment shader", "OpenGLShader", mLogger);
      glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
      OpenGLUtilities::CheckOpenGLError(
        "glShaderSource fragment shader", "OpenGLShader", mLogger);
      glCompileShader(fragmentShader);
      OpenGLUtilities::CheckOpenGLError(
        "glCompileShader fragment shader", "OpenGLShader", mLogger);

      GLint fragmentCompiled = 0;
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompiled);
      OpenGLUtilities::CheckOpenGLError(
        "glGetShaderiv fragment shader", "OpenGLShader", mLogger);
      glGetShaderInfoLog(fragmentShader, 1024, &fragLogLength, fragMessage);
      OpenGLUtilities::CheckOpenGLError(
        "glGetShaderInfoLog fragment shader", "OpenGLShader", mLogger);

      if (fragLogLength > 0)
      {
        mShaderLogs.mFragmentShaderLog = std::string(fragMessage);
      }

      if (fragmentCompiled != GL_TRUE)
      {
        glDeleteShader(vertexShader);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteShader vertex shader", "OpenGLShader", mLogger);
        glDeleteShader(fragmentShader);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteShader fragment shader", "OpenGLShader", mLogger);
        return;
      }

      mLogger->LogDebug(
        Log("Fragment shader compiled successfully", "OpenGLShader"));

      mID = glCreateProgram();

      OpenGLUtilities::CheckOpenGLError(
        "glCreateProgram", "OpenGLShader", mLogger);

      glAttachShader(mID, vertexShader);

      OpenGLUtilities::CheckOpenGLError(
        "glAttachShader vertex shader", "OpenGLShader", mLogger);

      glAttachShader(mID, fragmentShader);

      OpenGLUtilities::CheckOpenGLError(
        "glAttachShader fragment shader", "OpenGLShader", mLogger);

      GLuint geometryShader = -1;

      if (mGeometryShaderAsset.has_value())
      {
        const char* geometrySource =
          (dynamic_cast<GeometryShaderAsset&>(
             mGeometryShaderAsset.value()->GetAsset()))
            .GetFileContent()
            .c_str();

        GLsizei geomLogLength = 0;
        GLchar  geomMessage[1024] = "";

        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        OpenGLUtilities::CheckOpenGLError(
          "glCreateShader GL_GEOMETRY_SHADER", "OpenGLShader", mLogger);
        glShaderSource(geometryShader, 1, &geometrySource, nullptr);
        OpenGLUtilities::CheckOpenGLError(
          "glShaderSource", "OpenGLShader", mLogger);
        glCompileShader(geometryShader);
        OpenGLUtilities::CheckOpenGLError(
          "glCompileShader", "OpenGLShader", mLogger);

        GLint geometry_compiled = 0;
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &geometry_compiled);
        OpenGLUtilities::CheckOpenGLError(
          "glGetShaderiv", "OpenGLShader", mLogger);
        glGetShaderInfoLog(geometryShader, 1024, &geomLogLength, geomMessage);
        OpenGLUtilities::CheckOpenGLError(
          "glGetShaderInfoLog", "OpenGLShader", mLogger);

        if (geomLogLength > 0)
        {
          mShaderLogs.mGeometryShaderLog = std::string(geomMessage);
        }

        if (geometry_compiled != GL_TRUE)
        {
          glDeleteShader(vertexShader);
          OpenGLUtilities::CheckOpenGLError(
            "glDeleteShader vertexShader", "OpenGLShader", mLogger);
          glDeleteShader(fragmentShader);
          OpenGLUtilities::CheckOpenGLError(
            "glDeleteShader fragmentShader", "OpenGLShader", mLogger);
          glDeleteShader(geometryShader);
          OpenGLUtilities::CheckOpenGLError(
            "glDeleteShader geometryShader", "OpenGLShader", mLogger);
          return;
        }
      }

      glLinkProgram(mID);
      OpenGLUtilities::CheckOpenGLError(
        "glLinkProgram", "OpenGLShader", mLogger);

      GLint programLinked = 0;
      glGetProgramiv(mID, GL_LINK_STATUS, &programLinked);

      OpenGLUtilities::CheckOpenGLError(
        "glGetProgramiv", "OpenGLShader", mLogger);

      if (programLinked != GL_TRUE)
      {
        mLogger->LogError(Log("Shader program failed to link", "OpenGLShader"));
        glDeleteShader(vertexShader);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteShader vertex shader", "OpenGLShader", mLogger);
        glDeleteShader(fragmentShader);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteShader fragment shader", "OpenGLShader", mLogger);

        if (geometryShader != -1)
        {
          glDeleteShader(geometryShader);
          OpenGLUtilities::CheckOpenGLError(
            "glDeleteShader geometry shader", "OpenGLShader", mLogger);
        }
        glDeleteProgram(mID);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteProgram", "OpenGLShader", mLogger);
      }

      glDeleteShader(vertexShader);
      OpenGLUtilities::CheckOpenGLError(
        "glDeleteShader vertex shader", "OpenGLShader", mLogger);
      glDeleteShader(fragmentShader);
      OpenGLUtilities::CheckOpenGLError(
        "glDeleteShader fragment shader", "OpenGLShader", mLogger);

      if (geometryShader != -1)
      {
        glDeleteShader(geometryShader);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteShader geometry shader", "OpenGLShader", mLogger);
      }

      mSuccessfullyCompiled = true;
      GLint binaryLength = 0;
      glGetProgramiv(mID, GL_PROGRAM_BINARY_LENGTH, &binaryLength);
      OpenGLUtilities::CheckOpenGLError(
        "glGetProgramiv", "OpenGLShader", mLogger);
      mVramTracker->AddShaderMemory(binaryLength);
    }
    else
    {
      // TODO: log missing shader error
    }
  }

  auto
  OpenGLShader::IsCompiled() const -> bool
  {
    return mSuccessfullyCompiled;
  }

  auto
  OpenGLShader::GetID() const -> GLuint
  {
    return mID;
  }

  auto
  OpenGLShader::CreateParameters()
    -> std::unique_ptr<IShaderParameterCollection>
  {
    if (!mSuccessfullyCompiled)
    {
      return nullptr;
    }

    std::unique_ptr<IShaderParameterCollection> parameters =
      std::move(mShaderParameterCollectionFactory->Create());
    GLint parameterIndex = 0;
    GLint count = 0;

    GLint  size = 0; // size of the variable
    GLenum type = 0; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 64;  // maximum name length
    GLchar        name[bufSize]; // variable name in GLSL
    GLsizei       length = 0;    // name length

    glGetProgramiv(mID, GL_ACTIVE_UNIFORMS, &count);
    OpenGLUtilities::CheckOpenGLError(
      "glGetProgramiv GL_ACTIVE_UNIFORMS", "OpenGLShader", mLogger);

    for (parameterIndex = 0; parameterIndex < count; parameterIndex++)
    {
      glGetActiveUniform(
        mID, (GLuint)parameterIndex, bufSize, &length, &size, &type, name);
      OpenGLUtilities::CheckOpenGLError(
        "glGetActiveUniform", "OpenGLShader", mLogger);

      if (std::ranges::find(ReservedUniformNames, name) ==
          ReservedUniformNames.end())
      {
        parameters->mDefaultValueAdders.at(glTypeToDwarfShaderType.at(type))(
          name);
      }
    }

    return parameters;
  }

  auto
  OpenGLShader::GetVertexShaderAsset()
    -> std::optional<std::unique_ptr<IAssetReference>>&
  {
    return mVertexShaderAsset;
  }

  auto
  OpenGLShader::GetFragmentShaderAsset()
    -> std::optional<std::unique_ptr<IAssetReference>>&
  {
    return mFragmentShaderAsset;
  }

  auto
  OpenGLShader::GetGeometryShaderAsset()
    -> std::optional<std::unique_ptr<IAssetReference>>&
  {
    return mGeometryShaderAsset;
  }

  auto
  OpenGLShader::GetTessellationControlShaderAsset()
    -> std::optional<std::unique_ptr<IAssetReference>>&
  {
    return mTessellationControlShaderAsset;
  }

  auto
  OpenGLShader::GetTessellationEvaluationShaderAsset()
    -> std::optional<std::unique_ptr<IAssetReference>>&
  {
    return mTessellationEvaluationShaderAsset;
  }

  auto
  OpenGLShader::GetShaderLogs() const -> const ShaderLogs&
  {
    return mShaderLogs;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, bool value)
  {
    if (mUniformStates.contains(uniformName) &&
        (std::get<bool>(mUniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform1f(GetUniformLocation(uniformName), static_cast<GLfloat>(value));
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1f", "OpenGLRendererApi", mLogger);
    mUniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, int value)
  {
    if (mUniformStates.contains(uniformName) &&
        (std::get<int>(mUniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform1i(GetUniformLocation(uniformName), value);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1i", "OpenGLRendererApi", mLogger);
    mUniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, uint32_t value)
  {
    if (mUniformStates.contains(uniformName) &&
        (std::get<uint32_t>(mUniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform1ui(GetUniformLocation(uniformName), value);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1ui", "OpenGLRendererApi", mLogger);
    mUniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, float value)
  {
    if (mUniformStates.contains(uniformName) &&
        (std::get<float>(mUniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform1f(GetUniformLocation(uniformName), value);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1f", "OpenGLRendererApi", mLogger);
    mUniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string   uniformName,
                           TextureAsset& value,
                           unsigned int  textureCount)
  {
    // if (mTextureStates.contains(textureCount) &&
    //     (mTextureStates[textureCount] == value.GetTexture().GetTextureID()))
    // {
    //   // return;
    // }
    // else
    // {
    glActiveTexture(GL_TEXTURE0 + textureCount);
    OpenGLUtilities::CheckOpenGLError(
      "glActiveTexture", "OpenGLRendererApi", mLogger);
    glBindTexture(GL_TEXTURE_2D, value.GetTexture().GetTextureID());
    OpenGLUtilities::CheckOpenGLError("glBindTexture", "OpenGLShader", mLogger);
    mTextureStates[textureCount] = value.GetTexture().GetTextureID();
    //}

    if (mUniformStates.contains(uniformName) &&
        (std::get<int>(mUniformStates[uniformName]) ==
         static_cast<GLint>(textureCount)))
    {
      return;
    }

    glUniform1i(GetUniformLocation(uniformName),
                static_cast<GLint>(textureCount));
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1i", "OpenGLRendererApi", mLogger);
    glUniform1i(GetUniformLocation(uniformName),
                static_cast<GLint>(textureCount));
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1i", "OpenGLRendererApi", mLogger);
    mUniformStates[uniformName] = static_cast<GLint>(textureCount);
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, glm::vec2 value)
  {
    if (mUniformStates.contains(uniformName) &&
        (std::get<glm::vec2>(mUniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform2f(GetUniformLocation(uniformName), value.x, value.y);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform2f", "OpenGLRendererApi", mLogger);
    mUniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, glm::vec3 value)
  {
    if (mUniformStates.contains(uniformName) &&
        (std::get<glm::vec3>(mUniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform3f(GetUniformLocation(uniformName), value.x, value.y, value.z);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform3f", "OpenGLRendererApi", mLogger);
    mUniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, glm::vec4 value)
  {
    if (mUniformStates.contains(uniformName) &&
        (std::get<glm::vec4>(mUniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform4f(
      GetUniformLocation(uniformName), value.x, value.y, value.z, value.w);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform4f", "OpenGLRendererApi", mLogger);
    mUniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, glm::mat4 value)
  {
    if (mUniformStates.contains(uniformName) &&
        (std::get<glm::mat4>(mUniformStates[uniformName]) == value))
    {
      return;
    }

    glUniformMatrix4fv(
      GetUniformLocation(uniformName), 1, GL_FALSE, &value[0][0]);
    OpenGLUtilities::CheckOpenGLError(
      "glUniformMatrix4fv", "OpenGLRendererApi", mLogger);
    mUniformStates[uniformName] = value;
  }

  GLint
  OpenGLShader::GetUniformLocation(std::string uniformName)
  {
    if (!mUniformLocations.contains(uniformName))
    {
      mUniformLocations[uniformName] =
        glGetUniformLocation(mID, uniformName.c_str());
      OpenGLUtilities::CheckOpenGLError(
        "glGetUniformLocation", "OpenGLShader", mLogger);
    }

    return mUniformLocations[uniformName];
  }

  nlohmann::json
  OpenGLShader::Serialize()
  {
    nlohmann::json serializedShader;
    serializedShader["VertexShader"] =
      mVertexShaderAsset.has_value()
        ? mVertexShaderAsset.value()->GetUID().toString()
        : "";

    serializedShader["FragmentShader"] =
      mFragmentShaderAsset.has_value()
        ? mFragmentShaderAsset.value()->GetUID().toString()
        : "";

    serializedShader["GeometryShader"] =
      mGeometryShaderAsset.has_value()
        ? mGeometryShaderAsset.value()->GetUID().toString()
        : "";

    serializedShader["TessellationControlShader"] =
      mTessellationControlShaderAsset.has_value()
        ? mTessellationControlShaderAsset.value()->GetUID().toString()
        : "";

    serializedShader["TessellationEvaluationShader"] =
      mTessellationEvaluationShaderAsset.has_value()
        ? mTessellationEvaluationShaderAsset.value()->GetUID().toString()
        : "";

    return serializedShader;
  }

  bool
  OpenGLShader::CompareTo(const IShader& other) const
  {
    const OpenGLShader* otherShader = dynamic_cast<const OpenGLShader*>(&other);
    return mID < otherShader->GetID();
  }
}