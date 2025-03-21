#pragma once
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "pch.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.h"
#include "Editor/Stats/IEditorStats.h"
#include "Logging/IDwarfLogger.h"

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Platform/OpenGL/IOpenGLStateTracker.h"

namespace Dwarf
{
  class OpenGLRendererApi : public IRendererApi
  {
  private:
    std::shared_ptr<IAssetDatabase>      m_AssetDatabase;
    std::shared_ptr<IShaderRegistry>     m_ShaderRegistry;
    std::shared_ptr<IDwarfLogger>        m_Logger;
    std::shared_ptr<IEditorStats>        m_EditorStats;
    std::shared_ptr<IOpenGLStateTracker> m_StateTracker;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        m_ShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>       m_MeshFactory;
    std::shared_ptr<IMeshBufferFactory> m_MeshBufferFactory;

    std::shared_ptr<IShader>     m_ErrorShader;
    std::shared_ptr<IMeshBuffer> m_ScreenQuad;

  public:
    OpenGLRendererApi(std::shared_ptr<IAssetDatabase>      assetDatabase,
                      std::shared_ptr<IShaderRegistry>     shaderRegistry,
                      std::shared_ptr<IDwarfLogger>        logger,
                      std::shared_ptr<IEditorStats>        editorStats,
                      std::shared_ptr<IOpenGLStateTracker> stateTracker,
                      std::shared_ptr<IShaderSourceCollectionFactory>
                        shaderSourceCollectionFactory,
                      std::shared_ptr<IMeshFactory>       meshFactory,
                      std::shared_ptr<IMeshBufferFactory> meshBufferFactory);
    ~OpenGLRendererApi() override;

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
    RenderIndexed(IMeshBuffer& mesh,
                  IMaterial&   material,
                  ICamera&     camera,
                  glm::mat4    modelMatrix) override;
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

    void
    CustomBlit(IFramebuffer&            source,
               IFramebuffer&            destination,
               uint32_t                 sourceAttachment,
               uint32_t                 destinationAttachment,
               std::shared_ptr<IShader> shader) override;

    VRAMUsageBuffer
    QueryVRAMUsage() override;

    int32_t
    GetMaxSamples() override;
  };
}