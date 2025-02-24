#pragma once
#include "Core/Rendering/Shader/IShaderFactory.h"
#include "Editor/Stats/IEditorStats.h"
#include "Logging/IDwarfLogger.h"
#include "pch.h"

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"

namespace Dwarf
{
  class OpenGLRendererApi : public IRendererApi
  {
  private:
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;
    std::shared_ptr<IShaderFactory> m_ShaderFactory;
    std::shared_ptr<IDwarfLogger>   m_Logger;
    std::shared_ptr<IEditorStats>   m_EditorStats;

    std::shared_ptr<IShader> m_ErrorShader;

  public:
    OpenGLRendererApi(std::shared_ptr<IAssetDatabase> assetDatabase,
                      std::shared_ptr<IShaderFactory> shaderFactory,
                      std::shared_ptr<IDwarfLogger>   logger,
                      std::shared_ptr<IEditorStats>   editorStats);
    ~OpenGLRendererApi() override;

    void
    Init() override;

    void
    SetViewport(uint32_t x,
                uint32_t y,
                uint32_t width,
                uint32_t height) override;

    void
    SetClearColor(const glm::vec4& color) override;
    void
    Clear(unsigned int value) override;
    void
    Clear() override;

    void
    RenderIndexed(std::unique_ptr<IMesh>& mesh,
                  IMaterial&              material,
                  ICamera&                camera,
                  glm::mat4               modelMatrix) override;
    void
    ApplyComputeShader(std::shared_ptr<IComputeShader> computeShader,
                       std::shared_ptr<IFramebuffer>   fb,
                       uint32_t                        sourceAttachment,
                       uint32_t destinationAttachment) override;

    void
    Blit(IFramebuffer& source,
         IFramebuffer& destination,
         uint32_t      sourceAttachment,
         uint32_t      destinationAttachment,
         uint32_t      width,
         uint32_t      height) override;

    VRAMUsageBuffer
    QueryVRAMUsage() override;

    int32_t
    GetMaxSamples() override;
  };
}