#pragma once
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"
#include "pch.h"

#include <glad/glad.h>
#include <boost/di.hpp>
#include <boost/serialization/strong_typedef.hpp>
#include <boost/optional.hpp>
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/ShaderTypes.h"
#include "Core/Asset/Database/AssetReference.h"

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

    struct HandleShaderSourceVisitor;
    friend struct HandleShaderSourceVisitor;

  public:
    BOOST_DI_INJECT(OpenGLShader,
                    ShaderSourceCollection shaderSources,
                    std::shared_ptr<IShaderParameterCollectionFactory>
                      shaderParameterCollectionFactory);
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

    std::shared_ptr<AssetReference<VertexShaderAsset>>&
    GetVertexShaderAsset();

    std::shared_ptr<AssetReference<FragmentShaderAsset>>&
    GetFragmentShaderAsset();

    std::shared_ptr<AssetReference<GeometryShaderAsset>>&
    GetGeometryShaderAsset();

    std::shared_ptr<AssetReference<TessellationControlShaderAsset>>&
    GetTessellationControlShaderAsset();

    std::shared_ptr<AssetReference<TessellationEvaluationShaderAsset>>&
    GetTessellationEvaluationShaderAsset();

    static const std::array<std::string, 3> ReservedUniformNames;

  private:
    GLuint     m_ID = -1;
    ShaderLogs m_ShaderLogs;
    // Flag to determine if the shader has been successfully compiled.
    bool m_SuccessfullyCompiled;
    // Map of parameters that the shader uses.
    std::shared_ptr<IShaderParameterCollectionFactory>
      m_ShaderParameterCollectionFactory;

    std::shared_ptr<AssetReference<VertexShaderAsset>>   m_VertexShaderAsset;
    std::shared_ptr<AssetReference<GeometryShaderAsset>> m_GeometryShaderAsset;

    std::shared_ptr<AssetReference<TessellationControlShaderAsset>>
      m_TessellationControlShaderAsset;

    std::shared_ptr<AssetReference<TessellationEvaluationShaderAsset>>
      m_TessellationEvaluationShaderAsset;
    std::shared_ptr<AssetReference<FragmentShaderAsset>> m_FragmentShaderAsset;

    // Declare the visitor struct
    struct HandleShaderSourceVisitor
    {
      HandleShaderSourceVisitor(OpenGLShader& instance);

      void
      operator()(
        const std::shared_ptr<AssetReference<VertexShaderAsset>> source) const;

      void
      operator()(const std::shared_ptr<AssetReference<FragmentShaderAsset>>
                   source) const;

      void
      operator()(const std::shared_ptr<AssetReference<GeometryShaderAsset>>
                   source) const;

      void
      operator()(
        const std::shared_ptr<AssetReference<TessellationControlShaderAsset>>
          source) const;

      void
      operator()(
        const std::shared_ptr<AssetReference<TessellationEvaluationShaderAsset>>
          source) const;

    private:
      OpenGLShader& instance; // Reference to the MyClass instance
    };
  };
}