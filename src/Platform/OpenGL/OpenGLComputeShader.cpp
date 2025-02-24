#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Base.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "OpenGLComputeShader.h"

#define GL_SHADER_LOG_LENGTH (1024)

namespace Dwarf
{
  OpenGLComputeShader::OpenGLComputeShader(
    std::unique_ptr<IAssetReference>& computeShaderAsset,
    std::shared_ptr<IShaderParameterCollectionFactory>
                                  shaderParameterCollectionFactory,
    std::shared_ptr<IVramTracker> vramTracker)
    : m_ComputeShaderAsset(std::move(computeShaderAsset))
    , m_ShaderParameterCollectionFactory(shaderParameterCollectionFactory)
    , m_VramTracker(vramTracker)
  {
  }

  OpenGLComputeShader::~OpenGLComputeShader()
  {
    GLint binaryLength = 0;
    glGetProgramiv(m_ID, GL_PROGRAM_BINARY_LENGTH, &binaryLength);
    m_VramTracker->RemoveComputeMemory(binaryLength);
    glDeleteProgram(m_ID);
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
    m_SuccessfullyCompiled = false;

    ComputeShaderAsset& computeShaderAsset =
      (ComputeShaderAsset&)m_ComputeShaderAsset->GetAsset();

    if (computeShaderAsset.GetFileContent().length() > 0)
    {
      const char* shadercstr = computeShaderAsset.GetFileContent().c_str();

      GLsizei log_length = 0;
      GLchar  message[1024] = "";

      GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
      glShaderSource(shader, 1, &shadercstr, nullptr);
      glCompileShader(shader);

      GLint compiled;
      glGetShaderInfoLog(shader, 1024, &log_length, message);
      glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

      if (log_length > 0)
      {
        m_ComputeShaderLog = std::string(message);
      }
      if (compiled != GL_TRUE)
      {
        glDeleteShader(shader);
        return;
      }

      m_ID = glCreateProgram();

      glAttachShader(m_ID, shader);

      glLinkProgram(m_ID);

      glDeleteShader(shader);

      m_SuccessfullyCompiled = true;
      GLint binaryLength = 0;
      glGetProgramiv(m_ID, GL_PROGRAM_BINARY_LENGTH, &binaryLength);
      m_VramTracker->AddComputeMemory(binaryLength);
    }
    else
    {
      // TODO: log missing shader error
    }
  }

  bool
  OpenGLComputeShader::IsCompiled() const
  {
    return m_SuccessfullyCompiled;
  }

  GLuint
  OpenGLComputeShader::GetID() const
  {
    return m_ID;
  }

  const std::string&
  OpenGLComputeShader::GetLog() const
  {
    return m_ComputeShaderLog;
  }

  std::shared_ptr<IShaderParameterCollection>
  OpenGLComputeShader::GetParameters()
  {
    if (!m_SuccessfullyCompiled)
    {
      return nullptr;
    }
    std::shared_ptr<IShaderParameterCollection> parameters =
      m_ShaderParameterCollectionFactory->CreateShaderParameterCollection();
    GLint i;
    GLint count;

    GLint  size; // size of the variable
    GLenum type; // type of the variable (float, vec3 or mat4, etc)

    const GLsizei bufSize = 64;  // maximum name length
    GLchar        name[bufSize]; // variable name in GLSL
    GLsizei       length;        // name length

    glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &count);

    for (i = 0; i < count; i++)
    {
      glGetActiveUniform(m_ID, (GLuint)i, bufSize, &length, &size, &type, name);
      parameters->m_DefaultValueAdders.at(glTypeToDwarfShaderType.at(type))(
        name);
    }

    return parameters;
  }
}