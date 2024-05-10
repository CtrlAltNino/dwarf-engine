#pragma once
#include "pch.h"

#include <glad/glad.h>
#include "Core/Rendering/IShaderParameter.h"
#include "Core/Rendering/Shader.h"

namespace Dwarf
{
  struct ShaderLogs
  {
    std::string m_VertexShaderLog;
    std::string m_TessellationControlShaderLog;
    std::string m_TessellationEvaluationShaderLog;
    std::string m_GeometryShaderLog;
    std::string m_FragmentShaderLog;
  };

  struct ShaderSources
  {
    std::string m_VertexShaderSource;
    std::string m_TessellationControlShaderSource;
    std::string m_TessellationEvaluationShaderSource;
    std::string m_GeometryShaderSource;
    std::string m_FragmentShaderSource;
  };

  struct ShaderAssets
  {
    std::shared_ptr<UID> m_VertexShaderAsset;
    std::shared_ptr<UID> m_TessellationControlShaderAsset;
    std::shared_ptr<UID> m_TessellationEvaluationShaderAsset;
    std::shared_ptr<UID> m_GeometryShaderAsset;
    std::shared_ptr<UID> m_FragmentShaderAsset;
  };

  class OpenGLShader : public Shader
  {
  private:
    GLuint        m_ID = -1;
    ShaderSources m_ShaderSources;
    ShaderAssets  m_ShaderAssets;
    ShaderLogs    m_ShaderLogs;

  public:
    OpenGLShader();
    ~OpenGLShader() override;

    void
    SetVertexShader(std::shared_ptr<UID> vertexShader);
    void
    SetVertexShader(std::string_view vertexShader);

    void
    SetTesselaltionControlShader(
      std::shared_ptr<UID> tessellationControlShader);
    void
    SetTesselaltionControlShader(std::string_view tessellationControlShader);

    void
    SetTesselaltionEvaluationShader(
      std::shared_ptr<UID> tessellationEvaluationShader);
    void
    SetTesselaltionEvaluationShader(
      std::string_view tessellationEvaluationShader);

    void
    SetGeometryShader(std::shared_ptr<UID> geometryShader);
    void
    SetGeometryShader(std::string_view geometryShader);

    void
    SetFragmentShader(std::shared_ptr<UID> fragmentShader);
    void
    SetFragmentShader(std::string_view fragmentShader);

    std::shared_ptr<UID>
    GetVertexShader() const;
    std::shared_ptr<UID>
    GetFragmentShader() const;
    std::shared_ptr<UID>
    GetTesselaltionControlShader() const;
    std::shared_ptr<UID>
    GetTesselaltionEvaluationShader() const;
    std::shared_ptr<UID>
    GetGeometryShader() const;

    GLuint
    GetID() const;

    void
    Compile() override;

    std::map<std::string, std::shared_ptr<IShaderParameter>, std::less<>>
    GetParameters() override;

    static std::shared_ptr<OpenGLShader>
    CreateDefaultShader();
    static std::shared_ptr<OpenGLShader>
    CreateErrorShader();
    static std::shared_ptr<OpenGLShader>
    CreateGridShader();
    static std::shared_ptr<OpenGLShader>
    CreatePreviewShader();
    static std::shared_ptr<OpenGLShader>
    CreateIdShader();
    static std::shared_ptr<OpenGLShader>
    CreateWhiteShader();

    const ShaderLogs&
    GetShaderLogs() const;
    ShaderAssets&
    GetShaderAssets();

    static const std::array<std::string, 3> ReservedUniformNames;
  };
}