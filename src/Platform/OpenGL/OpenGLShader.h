#pragma once
#include "pch.h"

#include "Logging/IDwarfLogger.h"
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
                      shaderParameterCollectionFactory,
                    std::shared_ptr<IDwarfLogger> logger);
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

    std::optional<std::unique_ptr<IAssetReference>>&
    GetVertexShaderAsset();

    std::optional<std::unique_ptr<IAssetReference>>&
    GetFragmentShaderAsset();

    std::optional<std::unique_ptr<IAssetReference>>&
    GetGeometryShaderAsset();

    std::optional<std::unique_ptr<IAssetReference>>&
    GetTessellationControlShaderAsset();

    std::optional<std::unique_ptr<IAssetReference>>&
    GetTessellationEvaluationShaderAsset();

    static const std::array<std::string, 4> ReservedUniformNames;

    nlohmann::json
    Serialize() override;

  private:
    GLuint     m_ID = -1;
    ShaderLogs m_ShaderLogs;
    // Flag to determine if the shader has been successfully compiled.
    bool m_SuccessfullyCompiled;
    // Map of parameters that the shader uses.
    std::shared_ptr<IDwarfLogger> m_Logger;
    std::shared_ptr<IShaderParameterCollectionFactory>
      m_ShaderParameterCollectionFactory;

    std::optional<std::unique_ptr<IAssetReference>> m_VertexShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>> m_GeometryShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>>
      m_TessellationControlShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>>
      m_TessellationEvaluationShaderAsset;
    std::optional<std::unique_ptr<IAssetReference>> m_FragmentShaderAsset;
  };
}