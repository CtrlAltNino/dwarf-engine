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
#include "Core/Asset/AssetReference/IAssetReference.h"

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

    std::unique_ptr<IAssetReference<VertexShaderAsset>>&
    GetVertexShaderAsset();

    std::unique_ptr<IAssetReference<FragmentShaderAsset>>&
    GetFragmentShaderAsset();

    std::unique_ptr<IAssetReference<GeometryShaderAsset>>&
    GetGeometryShaderAsset();

    std::unique_ptr<IAssetReference<TessellationControlShaderAsset>>&
    GetTessellationControlShaderAsset();

    std::unique_ptr<IAssetReference<TessellationEvaluationShaderAsset>>&
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

    std::unique_ptr<IAssetReference<VertexShaderAsset>>   m_VertexShaderAsset;
    std::unique_ptr<IAssetReference<GeometryShaderAsset>> m_GeometryShaderAsset;

    std::unique_ptr<IAssetReference<TessellationControlShaderAsset>>
      m_TessellationControlShaderAsset;

    std::unique_ptr<IAssetReference<TessellationEvaluationShaderAsset>>
      m_TessellationEvaluationShaderAsset;
    std::unique_ptr<IAssetReference<FragmentShaderAsset>> m_FragmentShaderAsset;

    // Declare the visitor struct
    struct HandleShaderSourceVisitor
    {
      HandleShaderSourceVisitor(OpenGLShader& instance);

      void
      operator()(
        const std::unique_ptr<IAssetReference<VertexShaderAsset>> source) const;

      void
      operator()(const std::unique_ptr<IAssetReference<FragmentShaderAsset>>
                   source) const;

      void
      operator()(const std::unique_ptr<IAssetReference<GeometryShaderAsset>>
                   source) const;

      void
      operator()(
        const std::unique_ptr<IAssetReference<TessellationControlShaderAsset>>
          source) const;

      void
      operator()(
        const std::unique_ptr<
          IAssetReference<TessellationEvaluationShaderAsset>> source) const;

    private:
      OpenGLShader& instance; // Reference to the MyClass instance
    };
  };
}