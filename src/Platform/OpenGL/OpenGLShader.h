#pragma once
#include "pch.h"

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollectionFactory.h"

#include <glad/glad.h>
#include <boost/di.hpp>
#include <boost/serialization/strong_typedef.hpp>
#include <boost/optional.hpp>
#include "Core/Rendering/Shader/IShader.h"
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
                    std::unique_ptr<IShaderSourceCollection> shaderSources,
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

    std::optional<IAssetReference<VertexShaderAsset>>&
    GetVertexShaderAsset();

    std::optional<IAssetReference<FragmentShaderAsset>>&
    GetFragmentShaderAsset();

    std::optional<IAssetReference<GeometryShaderAsset>>&
    GetGeometryShaderAsset();

    std::optional<IAssetReference<TessellationControlShaderAsset>>&
    GetTessellationControlShaderAsset();

    std::optional<IAssetReference<TessellationEvaluationShaderAsset>>&
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

    std::optional<IAssetReference<VertexShaderAsset>>   m_VertexShaderAsset;
    std::optional<IAssetReference<GeometryShaderAsset>> m_GeometryShaderAsset;
    std::optional<IAssetReference<TessellationControlShaderAsset>>
      m_TessellationControlShaderAsset;
    std::optional<IAssetReference<TessellationEvaluationShaderAsset>>
      m_TessellationEvaluationShaderAsset;
    std::optional<IAssetReference<FragmentShaderAsset>> m_FragmentShaderAsset;

    // Declare the visitor struct
    struct HandleShaderSourceVisitor
    {
      HandleShaderSourceVisitor(OpenGLShader& instance);

      void
      operator()(const IAssetReference<VertexShaderAsset> source) const;

      void
      operator()(const IAssetReference<FragmentShaderAsset> source) const;

      void
      operator()(const IAssetReference<GeometryShaderAsset> source) const;

      void
      operator()(
        const IAssetReference<TessellationControlShaderAsset> source) const;

      void
      operator()(
        const IAssetReference<TessellationEvaluationShaderAsset> source) const;

    private:
      OpenGLShader& instance; // Reference to the MyClass instance
    };
  };
}