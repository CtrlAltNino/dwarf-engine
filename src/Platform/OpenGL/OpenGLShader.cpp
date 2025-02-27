#include "OpenGLShader.h"
#include "OpenGLUtilities.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Base.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Core/Rendering/Shader/ShaderTypes.h"
#include "Platform/OpenGL/OpenGLUtilities.h"
#include <memory>
#include <variant>
#include <fmt/format.h>

#define GL_SHADER_LOG_LENGTH (1024)

namespace Dwarf
{
  OpenGLShader::OpenGLShader(
    std::unique_ptr<IShaderSourceCollection> shaderSources,
    std::shared_ptr<IShaderParameterCollectionFactory>
                                  shaderParameterCollectionFactory,
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IVramTracker> vramTracker)
    : m_ShaderParameterCollectionFactory(shaderParameterCollectionFactory)
    , m_Logger(logger)
    , m_VramTracker(vramTracker)
  {
    for (std::unique_ptr<IAssetReference>& shaderSource :
         shaderSources->GetShaderSources())
    {
      // std::visit(HandleShaderSourceVisitor(*this), shaderSource);
      switch (shaderSource->GetType())
      {
        case Dwarf::ASSET_TYPE::VERTEX_SHADER:
          m_VertexShaderAsset = std::move(shaderSource);
          break;
        case Dwarf::ASSET_TYPE::FRAGMENT_SHADER:
          m_FragmentShaderAsset = std::move(shaderSource);
          break;
        case Dwarf::ASSET_TYPE::GEOMETRY_SHADER:
          m_GeometryShaderAsset = std::move(shaderSource);
          break;
        case Dwarf::ASSET_TYPE::TESC_SHADER:
          m_TessellationControlShaderAsset = std::move(shaderSource);
          break;
        case Dwarf::ASSET_TYPE::TESE_SHADER:
          m_TessellationEvaluationShaderAsset = std::move(shaderSource);
          break;
        default: break;
      }
    }
  }

  OpenGLShader::~OpenGLShader()
  {
    GLint binaryLength = 0;
    glGetProgramiv(m_ID, GL_PROGRAM_BINARY_LENGTH, &binaryLength);
    m_VramTracker->RemoveShaderMemory(binaryLength);
    OpenGLUtilities::CheckOpenGLError(
      "Errors before deleting shader", "OpenGLShader", m_Logger);
    glDeleteProgram(m_ID);
    OpenGLUtilities::CheckOpenGLError(
      "glDeleteProgram", "OpenGLShader", m_Logger);
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
    m_SuccessfullyCompiled = false;

    if (!m_VertexShaderAsset.has_value() || !m_FragmentShaderAsset.has_value())
    {
      return;
    }

    VertexShaderAsset& vertexShaderAsset =
      (VertexShaderAsset&)m_VertexShaderAsset.value()->GetAsset();

    FragmentShaderAsset& fragmentShaderAsset =
      (FragmentShaderAsset&)m_FragmentShaderAsset.value()->GetAsset();

    if (vertexShaderAsset.GetFileContent().length() > 0 &&
        fragmentShaderAsset.GetFileContent().length() > 0)
    {
      const char* vertexSource = vertexShaderAsset.GetFileContent().c_str();
      const char* fragmentSource = fragmentShaderAsset.GetFileContent().c_str();

      GLsizei vert_log_length = 0;
      GLchar  vert_message[1024] = "";

      GLsizei frag_log_length = 0;
      GLchar  frag_message[1024] = "";

      OpenGLUtilities::CheckOpenGLError(
        "Errors before compiling shader", "OpenGLShader", m_Logger);
      GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
      OpenGLUtilities::CheckOpenGLError(
        "glCreateShader vertex shader", "OpenGLShader", m_Logger);
      glShaderSource(vertexShader, 1, &vertexSource, nullptr);
      OpenGLUtilities::CheckOpenGLError(
        "glShaderSource vertex shader", "OpenGLShader", m_Logger);
      glCompileShader(vertexShader);
      OpenGLUtilities::CheckOpenGLError(
        "glCompileShader vertex shader", "OpenGLShader", m_Logger);

      GLint vertex_compiled = 33;
      glGetShaderInfoLog(vertexShader, 1024, &vert_log_length, vert_message);
      OpenGLUtilities::CheckOpenGLError(
        "glGetShaderInfoLog vertex shader", "OpenGLShader", m_Logger);
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertex_compiled);
      OpenGLUtilities::CheckOpenGLError(
        "glGetShaderiv vertex shader", "OpenGLShader", m_Logger);

      if (vert_log_length > 0)
      {
        m_ShaderLogs.m_VertexShaderLog = std::string(vert_message);
      }

      if (vertex_compiled != GL_TRUE)
      {
        glDeleteShader(vertexShader);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteShader vertex shader", "OpenGLShader", m_Logger);
        return;
      }
      else
      {
        m_Logger->LogDebug(
          Log("Vertex shader compiled successfully", "OpenGLShader"));
      }

      GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      OpenGLUtilities::CheckOpenGLError(
        "glCreateShader fragment shader", "OpenGLShader", m_Logger);
      glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
      OpenGLUtilities::CheckOpenGLError(
        "glShaderSource fragment shader", "OpenGLShader", m_Logger);
      glCompileShader(fragmentShader);
      OpenGLUtilities::CheckOpenGLError(
        "glCompileShader fragment shader", "OpenGLShader", m_Logger);

      GLint fragment_compiled;
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragment_compiled);
      OpenGLUtilities::CheckOpenGLError(
        "glGetShaderiv fragment shader", "OpenGLShader", m_Logger);
      glGetShaderInfoLog(fragmentShader, 1024, &frag_log_length, frag_message);
      OpenGLUtilities::CheckOpenGLError(
        "glGetShaderInfoLog fragment shader", "OpenGLShader", m_Logger);

      if (frag_log_length > 0)
      {
        m_ShaderLogs.m_FragmentShaderLog = std::string(frag_message);
      }

      if (fragment_compiled != GL_TRUE)
      {
        glDeleteShader(vertexShader);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteShader vertex shader", "OpenGLShader", m_Logger);
        glDeleteShader(fragmentShader);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteShader fragment shader", "OpenGLShader", m_Logger);
        return;
      }
      else
      {
        m_Logger->LogDebug(
          Log("Fragment shader compiled successfully", "OpenGLShader"));
      }

      m_ID = glCreateProgram();

      OpenGLUtilities::CheckOpenGLError(
        "glCreateProgram", "OpenGLShader", m_Logger);

      glAttachShader(m_ID, vertexShader);

      OpenGLUtilities::CheckOpenGLError(
        "glAttachShader vertex shader", "OpenGLShader", m_Logger);

      glAttachShader(m_ID, fragmentShader);

      OpenGLUtilities::CheckOpenGLError(
        "glAttachShader fragment shader", "OpenGLShader", m_Logger);

      GLuint geometryShader = -1;

      if (m_GeometryShaderAsset.has_value())
      {
        const char* geometrySource =
          ((GeometryShaderAsset&)m_GeometryShaderAsset.value()->GetAsset())
            .GetFileContent()
            .c_str();

        GLsizei geom_log_length = 0;
        GLchar  geom_message[1024] = "";

        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geometrySource, nullptr);
        glCompileShader(geometryShader);

        GLint geometry_compiled;
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &geometry_compiled);
        glGetShaderInfoLog(
          geometryShader, 1024, &geom_log_length, geom_message);

        if (geom_log_length > 0)
        {
          m_ShaderLogs.m_GeometryShaderLog = std::string(geom_message);
        }

        if (geometry_compiled != GL_TRUE)
        {
          glDeleteShader(vertexShader);
          glDeleteShader(fragmentShader);
          glDeleteShader(geometryShader);
          return;
        }
      }

      glLinkProgram(m_ID);
      OpenGLUtilities::CheckOpenGLError(
        "glLinkProgram", "OpenGLShader", m_Logger);

      GLint program_linked;
      glGetProgramiv(m_ID, GL_LINK_STATUS, &program_linked);

      OpenGLUtilities::CheckOpenGLError(
        "glGetProgramiv", "OpenGLShader", m_Logger);

      if (program_linked != GL_TRUE)
      {
        m_Logger->LogError(
          Log("Shader program failed to link", "OpenGLShader"));
        glDeleteShader(vertexShader);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteShader vertex shader", "OpenGLShader", m_Logger);
        glDeleteShader(fragmentShader);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteShader fragment shader", "OpenGLShader", m_Logger);

        if (geometryShader != -1)
        {
          glDeleteShader(geometryShader);
          OpenGLUtilities::CheckOpenGLError(
            "glDeleteShader geometry shader", "OpenGLShader", m_Logger);
        }
        glDeleteProgram(m_ID);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteProgram", "OpenGLShader", m_Logger);
      }

      glDeleteShader(vertexShader);
      OpenGLUtilities::CheckOpenGLError(
        "glDeleteShader vertex shader", "OpenGLShader", m_Logger);
      glDeleteShader(fragmentShader);
      OpenGLUtilities::CheckOpenGLError(
        "glDeleteShader fragment shader", "OpenGLShader", m_Logger);

      if (geometryShader != -1)
      {
        glDeleteShader(geometryShader);
        OpenGLUtilities::CheckOpenGLError(
          "glDeleteShader geometry shader", "OpenGLShader", m_Logger);
      }

      m_SuccessfullyCompiled = true;
      GLint binaryLength = 0;
      glGetProgramiv(m_ID, GL_PROGRAM_BINARY_LENGTH, &binaryLength);
      m_VramTracker->AddShaderMemory(binaryLength);
    }
    else
    {
      // TODO: log missing shader error
    }
  }

  bool
  OpenGLShader::IsCompiled() const
  {
    return m_SuccessfullyCompiled;
  }

  GLuint
  OpenGLShader::GetID() const
  {
    return m_ID;
  }

  // std::shared_ptr<OpenGLShader>
  // OpenGLShader::CreateDefaultShader()
  // {
  //   std::shared_ptr<OpenGLShader> defaultShader =
  //     std::make_shared<OpenGLShader>();
  //   defaultShader->SetVertexShader(
  //     FileHandler::ReadFile(Shader::GetDefaultShaderPath() /
  //     "default.vert"));
  //   defaultShader->SetFragmentShader(
  //     FileHandler::ReadFile(Shader::GetDefaultShaderPath() /
  //     "default.frag"));
  //   defaultShader->Compile();
  //   return defaultShader;
  // }

  // std::shared_ptr<OpenGLShader>
  // OpenGLShader::CreateErrorShader()
  // {
  //   std::shared_ptr<OpenGLShader> errorShader =
  //     std::make_shared<OpenGLShader>();
  //   errorShader->SetVertexShader(
  //     FileHandler::ReadFile(Shader::GetErrorShaderPath() / "error.vert"));
  //   errorShader->SetFragmentShader(
  //     FileHandler::ReadFile(Shader::GetErrorShaderPath() / "error.frag"));
  //   errorShader->Compile();
  //   return errorShader;
  // }

  // std::shared_ptr<OpenGLShader>
  // OpenGLShader::CreateGridShader()
  // {
  //   std::shared_ptr<OpenGLShader> gridShader =
  //   std::make_shared<OpenGLShader>(); gridShader->SetVertexShader(
  //     FileHandler::ReadFile(Shader::GetGridShaderPath() / "grid.vert"));
  //   gridShader->SetFragmentShader(
  //     FileHandler::ReadFile(Shader::GetGridShaderPath() / "grid.frag"));
  //   gridShader->Compile();
  //   return gridShader;
  // }

  // std::shared_ptr<OpenGLShader>
  // OpenGLShader::CreatePreviewShader()
  // {
  //   std::shared_ptr<OpenGLShader> previewShader =
  //     std::make_shared<OpenGLShader>();
  //   previewShader->SetVertexShader(
  //     FileHandler::ReadFile(Shader::GetPreviewShaderPath() /
  //     "preview.vert"));
  //   previewShader->SetFragmentShader(
  //     FileHandler::ReadFile(Shader::GetPreviewShaderPath() /
  //     "preview.frag"));
  //   previewShader->Compile();
  //   return previewShader;
  // }

  // std::shared_ptr<OpenGLShader>
  // OpenGLShader::CreateIdShader()
  // {
  //   std::shared_ptr<OpenGLShader> idShader =
  //   std::make_shared<OpenGLShader>(); idShader->SetVertexShader(
  //     FileHandler::ReadFile(Shader::GetIdShaderPath() / "id.vert"));
  //   idShader->SetFragmentShader(
  //     FileHandler::ReadFile(Shader::GetIdShaderPath() / "id.frag"));
  //   idShader->Compile();
  //   return idShader;
  // }

  // std::shared_ptr<OpenGLShader>
  // OpenGLShader::CreateWhiteShader()
  // {
  //   std::shared_ptr<OpenGLShader> whiteShader =
  //     std::make_shared<OpenGLShader>();
  //   whiteShader->SetVertexShader(
  //     FileHandler::ReadFile(Shader::GetOutlineShaderPath() / "white.vert"));
  //   whiteShader->SetFragmentShader(
  //     FileHandler::ReadFile(Shader::GetOutlineShaderPath() / "white.frag"));
  //   whiteShader->Compile();
  //   return whiteShader;
  // }

  std::unique_ptr<IShaderParameterCollection>
  OpenGLShader::CreateParameters()
  {
    if (!m_SuccessfullyCompiled)
    {
      return nullptr;
    }

    std::unique_ptr<IShaderParameterCollection> parameters = std::move(
      m_ShaderParameterCollectionFactory->CreateShaderParameterCollection());
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

      if (std::find(ReservedUniformNames.begin(),
                    ReservedUniformNames.end(),
                    name) == ReservedUniformNames.end())
      {
        parameters->m_DefaultValueAdders.at(glTypeToDwarfShaderType.at(type))(
          name);
      }
    }

    return parameters;
  }

  std::optional<std::unique_ptr<IAssetReference>>&
  OpenGLShader::GetVertexShaderAsset()
  {
    return m_VertexShaderAsset;
  }

  std::optional<std::unique_ptr<IAssetReference>>&
  OpenGLShader::GetFragmentShaderAsset()
  {
    return m_FragmentShaderAsset;
  }

  std::optional<std::unique_ptr<IAssetReference>>&
  OpenGLShader::GetGeometryShaderAsset()
  {
    return m_GeometryShaderAsset;
  }

  std::optional<std::unique_ptr<IAssetReference>>&
  OpenGLShader::GetTessellationControlShaderAsset()
  {
    return m_TessellationControlShaderAsset;
  }

  std::optional<std::unique_ptr<IAssetReference>>&
  OpenGLShader::GetTessellationEvaluationShaderAsset()
  {
    return m_TessellationEvaluationShaderAsset;
  }

  const ShaderLogs&
  OpenGLShader::GetShaderLogs() const
  {
    return m_ShaderLogs;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, bool value)
  {
    if (m_UniformStates.contains(uniformName) &&
        (std::get<bool>(m_UniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform1f(GetUniformLocation(uniformName), value);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1f", "OpenGLRendererApi", m_Logger);
    m_UniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, int value)
  {
    if (m_UniformStates.contains(uniformName) &&
        (std::get<int>(m_UniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform1i(GetUniformLocation(uniformName), value);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1i", "OpenGLRendererApi", m_Logger);
    m_UniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, uint32_t value)
  {
    if (m_UniformStates.contains(uniformName) &&
        (std::get<uint32_t>(m_UniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform1ui(GetUniformLocation(uniformName), value);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1ui", "OpenGLRendererApi", m_Logger);
    m_UniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, float value)
  {
    if (m_UniformStates.contains(uniformName) &&
        (std::get<float>(m_UniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform1f(GetUniformLocation(uniformName), value);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1f", "OpenGLRendererApi", m_Logger);
    m_UniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string   uniformName,
                           TextureAsset& value,
                           unsigned int  textureCount)
  {
    if (m_TextureStates.contains(textureCount) &&
        (m_TextureStates[textureCount] == value.GetTexture().GetTextureID()))
    {
      // return;
    }
    else
    {
      glActiveTexture(GL_TEXTURE0 + textureCount);
      OpenGLUtilities::CheckOpenGLError(
        "glActiveTexture", "OpenGLRendererApi", m_Logger);
      glBindTexture(GL_TEXTURE_2D, value.GetTexture().GetTextureID());
      m_TextureStates[textureCount] = value.GetTexture().GetTextureID();
    }

    if (m_UniformStates.contains(uniformName) &&
        (std::get<int>(m_UniformStates[uniformName]) ==
         static_cast<GLint>(textureCount)))
    {
      return;
    }

    glUniform1i(GetUniformLocation(uniformName),
                static_cast<GLint>(textureCount));
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1i", "OpenGLRendererApi", m_Logger);
    glUniform1i(GetUniformLocation(uniformName),
                static_cast<GLint>(textureCount));
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1i", "OpenGLRendererApi", m_Logger);
    m_UniformStates[uniformName] = static_cast<GLint>(textureCount);
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, glm::vec2 value)
  {
    if (m_UniformStates.contains(uniformName) &&
        (std::get<glm::vec2>(m_UniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform2f(GetUniformLocation(uniformName), value.x, value.y);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform2f", "OpenGLRendererApi", m_Logger);
    m_UniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, glm::vec3 value)
  {
    if (m_UniformStates.contains(uniformName) &&
        (std::get<glm::vec3>(m_UniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform3f(GetUniformLocation(uniformName), value.x, value.y, value.z);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform3f", "OpenGLRendererApi", m_Logger);
    m_UniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, glm::vec4 value)
  {
    if (m_UniformStates.contains(uniformName) &&
        (std::get<glm::vec4>(m_UniformStates[uniformName]) == value))
    {
      return;
    }

    glUniform4f(
      GetUniformLocation(uniformName), value.x, value.y, value.z, value.w);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform4f", "OpenGLRendererApi", m_Logger);
    m_UniformStates[uniformName] = value;
  }

  void
  OpenGLShader::SetUniform(std::string uniformName, glm::mat4 value)
  {
    if (m_UniformStates.contains(uniformName) &&
        (std::get<glm::mat4>(m_UniformStates[uniformName]) == value))
    {
      return;
    }

    glUniformMatrix4fv(
      GetUniformLocation(uniformName), 1, GL_FALSE, &value[0][0]);
    OpenGLUtilities::CheckOpenGLError(
      "glUniformMatrix4fv", "OpenGLRendererApi", m_Logger);
    m_UniformStates[uniformName] = value;
  }

  GLuint
  OpenGLShader::GetUniformLocation(std::string uniformName)
  {
    if (!m_UniformLocations.contains(uniformName))
    {
      m_UniformLocations[uniformName] =
        glGetUniformLocation(m_ID, uniformName.c_str());
    }

    return m_UniformLocations[uniformName];
  }

  nlohmann::json
  OpenGLShader::Serialize()
  {
    nlohmann::json serializedShader;
    serializedShader["VertexShader"] =
      m_VertexShaderAsset.has_value()
        ? m_VertexShaderAsset.value()->GetUID().ToString()
        : "";

    serializedShader["FragmentShader"] =
      m_FragmentShaderAsset.has_value()
        ? m_FragmentShaderAsset.value()->GetUID().ToString()
        : "";

    serializedShader["GeometryShader"] =
      m_GeometryShaderAsset.has_value()
        ? m_GeometryShaderAsset.value()->GetUID().ToString()
        : "";

    serializedShader["TessellationControlShader"] =
      m_TessellationControlShaderAsset.has_value()
        ? m_TessellationControlShaderAsset.value()->GetUID().ToString()
        : "";

    serializedShader["TessellationEvaluationShader"] =
      m_TessellationEvaluationShaderAsset.has_value()
        ? m_TessellationEvaluationShaderAsset.value()->GetUID().ToString()
        : "";

    return serializedShader;
  }

  bool
  OpenGLShader::CompareTo(const IShader& other) const
  {
    const OpenGLShader* otherShader = dynamic_cast<const OpenGLShader*>(&other);
    return m_ID < otherShader->GetID();
  }
}