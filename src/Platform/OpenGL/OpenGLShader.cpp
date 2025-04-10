#include "Core/Asset/Database/IAssetDatabase.h"
#include "pch.h"

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Base.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "OpenGLShader.h"
#include "OpenGLUtilities.h"
#include "Platform/OpenGL/OpenGLUtilities.h"
#include <glad/glad.h>

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
    { GL_FLOAT, ShaderParameterType::FLOAT },
    { GL_FLOAT_VEC2, ShaderParameterType::VEC2 },
    { GL_FLOAT_VEC3, ShaderParameterType::VEC3 },
    { GL_FLOAT_VEC4, ShaderParameterType::VEC4 },
    { GL_INT, ShaderParameterType::INTEGER },
    { GL_INT_VEC2, ShaderParameterType::IVEC2 },
    { GL_INT_VEC3, ShaderParameterType::IVEC3 },
    { GL_INT_VEC4, ShaderParameterType::IVEC4 },
    { GL_UNSIGNED_INT, ShaderParameterType::UNSIGNED_INTEGER },
    { GL_UNSIGNED_INT_VEC2, ShaderParameterType::UVEC2 },
    { GL_UNSIGNED_INT_VEC3, ShaderParameterType::UVEC3 },
    { GL_UNSIGNED_INT_VEC4, ShaderParameterType::UVEC4 },
    { GL_BOOL, ShaderParameterType::BOOLEAN },
    { GL_FLOAT_MAT3, ShaderParameterType::MAT3 },
    { GL_FLOAT_MAT4, ShaderParameterType::MAT4 },
    { GL_SAMPLER_2D, ShaderParameterType::TEX2D },
    { GL_SAMPLER_2D_MULTISAMPLE, ShaderParameterType::TEX2D }
  };

  void
  OpenGLShader::Compile()
  {
    mSuccessfullyCompiled = false;
    mUniformLocations.clear();

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
        GLint logLength = 0;
        glGetProgramiv(mID, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> infoLog(logLength);
        glGetProgramInfoLog(mID, logLength, nullptr, infoLog.data());

        mLogger->LogInfo(Log(infoLog.data(), "OpenGLShader"));
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
        return;
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

  void
  OpenGLShader::SetParameter(std::string          identifier,
                             ShaderParameterValue parameter)
  {
    mUniformStatesDraft[identifier] = parameter;
  }

  auto
  OpenGLShader::GetShaderLogs() const -> const ShaderLogs&
  {
    return mShaderLogs;
  }

  void
  OpenGLShader::ResetUniformBindings()
  {
    mUniformStates.clear();
    mTextureStates.clear();
  }

  void
  OpenGLShader::UploadParameters()
  {
    int textureCount = 0;
    for (const auto& pair : mUniformStatesDraft)
    {
      auto it = mUniformStates.find(pair.first);
      if (it != mUniformStates.end() && it->second == pair.second)
      {
        continue;
      }

      std::visit(
        [&, pair](auto&& val)
        {
          using T = std::decay_t<decltype(val)>;

          GLint uniformLocation = GetUniformLocation(pair.first);
          // Float parameter
          if constexpr (std::is_same_v<T, float>)
          {
            glUniform1f(uniformLocation, static_cast<GLfloat>(val));
            OpenGLUtilities::CheckOpenGLError(
              "glUniform1f", "OpenGLRendererApi", mLogger);
          }
          // Vec2 parameter
          if constexpr (std::is_same_v<T, glm::vec2>)
          {
            auto param(static_cast<glm::vec2>(val));
            glUniform2f(uniformLocation, param.x, param.y);
            OpenGLUtilities::CheckOpenGLError(
              "glUniform2f", "OpenGLRendererApi", mLogger);
          }
          // Vec3 parameter
          if constexpr (std::is_same_v<T, glm::vec3>)
          {
            auto param(static_cast<glm::vec3>(val));
            glUniform3f(uniformLocation, param.x, param.y, param.z);
            OpenGLUtilities::CheckOpenGLError(
              "glUniform3f", "OpenGLRendererApi", mLogger);
          }
          // Vec4 parameter
          if constexpr (std::is_same_v<T, glm::vec4>)
          {
            auto param(static_cast<glm::vec4>(val));
            glUniform4f(uniformLocation, param.x, param.y, param.z, param.w);
            OpenGLUtilities::CheckOpenGLError(
              "glUniform4f", "OpenGLRendererApi", mLogger);
          }
          // Int parameter
          if constexpr (std::is_same_v<T, int>)
          {
            auto param(static_cast<int>(val));
            glUniform1i(uniformLocation, param);
            OpenGLUtilities::CheckOpenGLError(
              "glUniform1i", "OpenGLRendererApi", mLogger);
          }
          // iVec2 parameter
          if constexpr (std::is_same_v<T, glm::ivec2>)
          {
            auto param(static_cast<glm::ivec2>(val));
            glUniform2i(uniformLocation, param.x, param.y);
            OpenGLUtilities::CheckOpenGLError(
              "glUniform2i", "OpenGLRendererApi", mLogger);
          }
          // iVec3 parameter
          if constexpr (std::is_same_v<T, glm::ivec3>)
          {
            auto param(static_cast<glm::ivec3>(val));
            glUniform3i(uniformLocation, param.x, param.y, param.z);
            OpenGLUtilities::CheckOpenGLError(
              "glUniform3i", "OpenGLRendererApi", mLogger);
          }
          // iVec4 parameter
          if constexpr (std::is_same_v<T, glm::ivec4>)
          {
            auto param(static_cast<glm::ivec4>(val));
            glUniform4i(uniformLocation, param.x, param.y, param.z, param.w);
            OpenGLUtilities::CheckOpenGLError(
              "glUniform4i", "OpenGLRendererApi", mLogger);
          }
          // unsigned int parameter
          if constexpr (std::is_same_v<T, uint32_t>)
          {
            auto param(static_cast<uint32_t>(val));
            glUniform1ui(uniformLocation, param);
            OpenGLUtilities::CheckOpenGLError(
              "glUniform1ui", "OpenGLRendererApi", mLogger);
          }
          // uvec2 parameter
          if constexpr (std::is_same_v<T, glm::uvec2>)
          {
            auto param(static_cast<glm::uvec2>(val));
            glUniform2ui(uniformLocation, param.x, param.y);
            OpenGLUtilities::CheckOpenGLError(
              "glUniform2ui", "OpenGLRendererApi", mLogger);
          }
          // uvec3 parameter
          if constexpr (std::is_same_v<T, glm::uvec3>)
          {
            auto param(static_cast<glm::uvec3>(val));
            glUniform3ui(uniformLocation, param.x, param.y, param.z);
            OpenGLUtilities::CheckOpenGLError(
              "glUniform3ui", "OpenGLRendererApi", mLogger);
          }
          // uvec4 parameter
          if constexpr (std::is_same_v<T, glm::uvec4>)
          {
            auto param(static_cast<glm::uvec4>(val));
            glUniform4ui(uniformLocation, param.x, param.y, param.z, param.w);
            OpenGLUtilities::CheckOpenGLError(
              "glUniform4ui", "OpenGLRendererApi", mLogger);
          }
          // Bool parameter
          if constexpr (std::is_same_v<T, bool>)
          {
            glUniform1f(uniformLocation, static_cast<GLfloat>(val));
            OpenGLUtilities::CheckOpenGLError(
              "glUniform1f", "OpenGLRendererApi", mLogger);
          }
          // Mat3 parameter
          if constexpr (std::is_same_v<T, glm::mat3>)
          {
            auto param(static_cast<glm::mat3>(val));
            glUniformMatrix3fv(
              uniformLocation, 1, GL_FALSE, glm::value_ptr(param));
            OpenGLUtilities::CheckOpenGLError(
              "glUniformMatrix3fv", "OpenGLRendererApi", mLogger);
          }
          // Mat4 parameter
          if constexpr (std::is_same_v<T, glm::mat4>)
          {
            auto param(static_cast<glm::mat4>(val));
            glUniformMatrix4fv(
              uniformLocation, 1, GL_FALSE, glm::value_ptr(param));
            OpenGLUtilities::CheckOpenGLError(
              "glUniformMatrix4fv", "OpenGLRendererApi", mLogger);
          }
          // Texture2D parameter
          if constexpr (std::is_same_v<T, std::shared_ptr<ITexture>>)
          {
            auto param(static_cast<std::shared_ptr<ITexture>>(val));
            glActiveTexture(GL_TEXTURE0 + textureCount);
            OpenGLUtilities::CheckOpenGLError(
              "glActiveTexture", "OpenGLRendererApi", mLogger);
            glBindTexture(GL_TEXTURE_2D, param->GetTextureID());
            OpenGLUtilities::CheckOpenGLError(
              "glBindTexture", "OpenGLShader", mLogger);
            mTextureStates[textureCount] = param->GetTextureID();

            /*if (mUniformStates.contains(pair.first) &&
                (std::get<int>(mUniformStates[pair.first]) ==
                 static_cast<GLint>(textureCount)))
            {
              return;
            }*/

            glUniform1i(uniformLocation, static_cast<GLint>(textureCount));
            OpenGLUtilities::CheckOpenGLError(
              "glUniform1i", "OpenGLRendererApi", mLogger);
            mUniformStates[pair.first] = static_cast<GLint>(textureCount);
            textureCount++;
          }
        },
        pair.second);
    }

    mUniformStates = mUniformStatesDraft;
  }

  auto
  OpenGLShader::GetUniformLocation(std::string uniformName) -> GLint
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

  auto
  OpenGLShader::CompareTo(const IShader& other) const -> bool
  {
    const auto* otherShader = dynamic_cast<const OpenGLShader*>(&other);
    return mID < otherShader->GetID();
  }
}