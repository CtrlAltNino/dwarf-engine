#include "OpenGLShader.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Base.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "Core/Rendering/Shader/ShaderTypes.h"
#include <memory>
#include <variant>

#define GL_SHADER_LOG_LENGTH (1024)

namespace Dwarf
{
  OpenGLShader::OpenGLShader(
    std::unique_ptr<IShaderSourceCollection> shaderSources,
    std::shared_ptr<IShaderParameterCollectionFactory>
      shaderParameterCollectionFactory)
    : m_ShaderParameterCollectionFactory(shaderParameterCollectionFactory)
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
      }
    }
  }

  OpenGLShader::~OpenGLShader()
  {
    glDeleteProgram(m_ID);
  }

  const std::array<std::string, 3> OpenGLShader::ReservedUniformNames = {
    "modelMatrix",
    "viewMatrix",
    "projectionMatrix",
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

      GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertexShader, 1, &vertexSource, nullptr);
      glCompileShader(vertexShader);

      GLint vertex_compiled;
      glGetShaderInfoLog(vertexShader, 1024, &vert_log_length, vert_message);
      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertex_compiled);

      if (vert_log_length > 0)
      {
        m_ShaderLogs.m_VertexShaderLog = std::string(vert_message);
      }
      if (vertex_compiled != GL_TRUE)
      {
        glDeleteShader(vertexShader);
        return;
      }

      GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
      glCompileShader(fragmentShader);

      GLint fragment_compiled;
      glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragment_compiled);
      glGetShaderInfoLog(fragmentShader, 1024, &frag_log_length, frag_message);

      if (frag_log_length > 0)
      {
        m_ShaderLogs.m_FragmentShaderLog = std::string(frag_message);
      }

      if (fragment_compiled != GL_TRUE)
      {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return;
      }

      m_ID = glCreateProgram();

      glAttachShader(m_ID, vertexShader);
      glAttachShader(m_ID, fragmentShader);

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

      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      if (std::cmp_not_equal(geometryShader, -1))
      {
        glDeleteShader(geometryShader);
      }

      m_SuccessfullyCompiled = true;
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

  nlohmann::json
  OpenGLShader::Serialize() const
  {
    nlohmann::json serializedShader;
    serializedShader["VertexShader"] =
      m_VertexShaderAsset.has_value()
        ? m_VertexShaderAsset.value()->GetUID().ToString()
        : "-1";

    serializedShader["FragmentShader"] =
      m_FragmentShaderAsset.has_value()
        ? m_FragmentShaderAsset.value()->GetUID().ToString()
        : "-1";

    serializedShader["GeometryShader"] =
      m_GeometryShaderAsset.has_value()
        ? m_GeometryShaderAsset.value()->GetUID().ToString()
        : "-1";

    serializedShader["TessellationControlShader"] =
      m_TessellationControlShaderAsset.has_value()
        ? m_TessellationControlShaderAsset.value()->GetUID().ToString()
        : "-1";

    serializedShader["TessellationEvaluationShader"] =
      m_TessellationEvaluationShaderAsset.has_value()
        ? m_TessellationEvaluationShaderAsset.value()->GetUID().ToString()
        : "-1";

    return serializedShader;
  }
}