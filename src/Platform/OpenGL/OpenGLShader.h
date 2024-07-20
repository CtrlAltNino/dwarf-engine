#pragma once
#include "Core/Asset/AssetComponents.h"
#include "pch.h"

#include <glad/glad.h>
#include <boost/serialization/strong_typedef.hpp>
#include <boost/optional.hpp>
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/ShaderTypes.h"
#include "Core/Asset/AssetReference.h"

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

  class OpenGLShader : public IShader
  {
  private:
    GLuint     m_ID = -1;
    ShaderLogs m_ShaderLogs;
    // Flag to determine if the shader has been successfully compiled.
    bool m_SuccessfullyCompiled;
    // Map of parameters that the shader uses.
    std::shared_ptr<IShaderParameterCollection> m_Parameters;

    std::shared_ptr<AssetReference<VertexShaderAsset>> m_VertexShaderAsset;
    boost::optional<std::shared_ptr<AssetReference<GeometryShaderAsset>>>
      m_GeometryShaderAsset;
    boost::optional<
      std::shared_ptr<AssetReference<TessellationControlShaderAsset>>>
      m_TessellationControlShaderAsset;
    boost::optional<
      std::shared_ptr<AssetReference<TessellationEvaluationShaderAsset>>>
      m_TessellationEvaluationShaderAsset;
    std::shared_ptr<AssetReference<FragmentShaderAsset>> m_FragmentShaderAsset;

  public:
    BOOST_DI_INJECT(
      OpenGLShader,
      std::shared_ptr<AssetReference<VertexShaderAsset>>   vertexShaderAsset,
      std::shared_ptr<AssetReference<FragmentShaderAsset>> fragmentShaderAsset,
      boost::optional<std::shared_ptr<AssetReference<GeometryShaderAsset>>>
        geometryShaderAsset = boost::none,
      boost::optional<
        std::shared_ptr<AssetReference<TessellationControlShaderAsset>>>
        tessellationControlShaderAsset = boost::none,
      boost::optional<
        std::shared_ptr<AssetReference<TessellationEvaluationShaderAsset>>>
        tessellationEvaluationShaderAsset = boost::none);
    ~OpenGLShader() = default;
    GLuint
    GetID() const;

    void
    Compile() override;

    std::shared_ptr<IShaderParameterCollection>
    GetParameters() override;

    const ShaderLogs&
    GetShaderLogs() const;

    static const std::array<std::string, 3> ReservedUniformNames;
  };
}