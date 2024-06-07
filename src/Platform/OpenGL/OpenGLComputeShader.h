#pragma once

#include <glad/glad.h>
#include "Core/Asset/IAssetDatabase.h"
#include "Core/Rendering/IComputeShader.h"
#include <Core/Rendering/IShaderParameter.h>
#include <memory>

namespace Dwarf
{
  class OpenGLComputeShader : public IComputeShader
  {
  private:
    GLuint                          m_ID = -1;
    std::string                     m_ComputeShaderLog;
    std::string                     m_ComputeShaderSource;
    std::shared_ptr<UID>            m_ComputeShaderAsset;
    bool                            m_SuccessfullyCompiled = false;
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;

  public:
    OpenGLComputeShader(std::shared_ptr<IAssetDatabase> assetDatabase);
    ~OpenGLComputeShader() override = default;

    void
    SetComputeShader(std::shared_ptr<UID> computeShader);
    void
    SetComputeShader(std::string_view computeShader);
    std::map<std::string, std::shared_ptr<IShaderParameter>, std::less<>>
    GetParameters();
    void
    UpdateParameters() override;

    std::shared_ptr<UID>
    GetComputeShader() const;

    GLuint
    GetID() const;

    void
    Compile() override;

    bool
    IsCompiled() const override;

    static std::shared_ptr<OpenGLComputeShader>
    CreatePropagationShader();
    static std::shared_ptr<OpenGLComputeShader>
    CreateFinalizationShader();

    const std::string&
    GetLog() const;
    std::shared_ptr<UID>&
    GetAsset();
  };
}