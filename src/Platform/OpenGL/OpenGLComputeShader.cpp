#include "OpenGLComputeShader.h"

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Base.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"

#define GL_SHADER_LOG_LENGTH (1024)

namespace Dwarf
{
  OpenGLComputeShader::OpenGLComputeShader(
    std::unique_ptr<IAssetReference>& computeShaderAsset,
    std::shared_ptr<IShaderParameterCollectionFactory>
                                  shaderParameterCollectionFactory,
    std::shared_ptr<IVramTracker> vramTracker)
    : mComputeShaderAsset(std::move(computeShaderAsset))
    , mShaderParameterCollectionFactory(
        std::move(shaderParameterCollectionFactory))
    , mVramTracker(std::move(vramTracker))
  {
  }

  OpenGLComputeShader::~OpenGLComputeShader()
  {
    GLint binaryLength = 0;
    glGetProgramiv(mID, GL_PROGRAM_BINARY_LENGTH, &binaryLength);
    mVramTracker->RemoveComputeMemory(binaryLength);
    glDeleteProgram(mID);
  }

  const std::map<GLenum, ShaderParameterType> glTypeToDwarfShaderType = {
    { GL_BOOL, ShaderParameterType::BOOLEAN },
    { GL_INT, ShaderParameterType::INTEGER },
    { GL_FLOAT, ShaderParameterType::FLOAT },
    { GL_FLOAT_VEC2, ShaderParameterType::VEC2 },
    { GL_FLOAT_VEC3, ShaderParameterType::VEC3 },
    { GL_FLOAT_VEC4, ShaderParameterType::VEC4 },
    { GL_SAMPLER_2D, ShaderParameterType::TEX2D }
  };

  void
  OpenGLComputeShader::Compile()
  {
    mSuccessfullyCompiled = false;

    auto& computeShaderAsset =
      dynamic_cast<ComputeShaderAsset&>(mComputeShaderAsset->GetAsset());

    if (computeShaderAsset.GetFileContent().length() > 0)
    {
      const char* shadercstr = computeShaderAsset.GetFileContent().c_str();

      GLsizei logLength = 0;
      GLchar  message[1024] = "";

      GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
      glShaderSource(shader, 1, &shadercstr, nullptr);
      glCompileShader(shader);

      GLint compiled = 0;
      glGetShaderInfoLog(shader, 1024, &logLength, message);
      glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

      if (logLength > 0)
      {
        mComputeShaderLog = std::string(message);
      }
      if (compiled != GL_TRUE)
      {
        glDeleteShader(shader);
        return;
      }

      mID = glCreateProgram();

      glAttachShader(mID, shader);

      glLinkProgram(mID);

      glDeleteShader(shader);

      mSuccessfullyCompiled = true;
      GLint binaryLength = 0;
      glGetProgramiv(mID, GL_PROGRAM_BINARY_LENGTH, &binaryLength);
      mVramTracker->AddComputeMemory(binaryLength);
    }
    else
    {
      // TODO: log missing shader error
    }
  }

  auto
  OpenGLComputeShader::IsCompiled() const -> bool
  {
    return mSuccessfullyCompiled;
  }

  auto
  OpenGLComputeShader::GetID() const -> GLuint
  {
    return mID;
  }

  auto
  OpenGLComputeShader::GetLog() const -> const std::string&
  {
    return mComputeShaderLog;
  }

  auto
  OpenGLComputeShader::GetParameters()
    -> std::shared_ptr<IShaderParameterCollection>
  {
    if (!mSuccessfullyCompiled)
    {
      return nullptr;
    }
    std::shared_ptr<IShaderParameterCollection> parameters =
      mShaderParameterCollectionFactory->Create();
    GLint i = 0;
    GLint count = 0;

    GLint  size = 0; // size of the variable
    GLenum type = 0; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 64;  // maximum name length
    GLchar        name[bufSize]; // variable name in GLSL
    GLsizei       length = 0;    // name length

    glGetProgramiv(mID, GL_ACTIVE_UNIFORMS, &count);

    for (i = 0; i < count; i++)
    {
      glGetActiveUniform(mID, (GLuint)i, bufSize, &length, &size, &type, name);
      parameters->mDefaultValueAdders.at(glTypeToDwarfShaderType.at(type))(
        name);
    }

    return parameters;
  }
}