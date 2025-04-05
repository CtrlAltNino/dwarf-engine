#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Rendering/Shader/IShader.h"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.h"
#include "Editor/Stats/IEditorStats.h"
#include "Logging/IDwarfLogger.h"
#include "Platform/OpenGL/IOpenGLStateTracker.h"

namespace Dwarf
{
  class OpenGLRendererApi : public IRendererApi
  {
  private:
    std::shared_ptr<IAssetDatabase>      mAssetDatabase;
    std::shared_ptr<IShaderRegistry>     mShaderRegistry;
    std::shared_ptr<IDwarfLogger>        mLogger;
    std::shared_ptr<IEditorStats>        mEditorStats;
    std::shared_ptr<IOpenGLStateTracker> mStateTracker;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        mShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>       mMeshFactory;
    std::shared_ptr<IMeshBufferFactory> mMeshBufferFactory;

    std::shared_ptr<IShader>     mErrorShader;
    std::shared_ptr<IMeshBuffer> mScreenQuad;

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

    /**
     * @brief Sets the viewport
     *
     * @param x X position of the viewport
     * @param y Y position of the viewport
     * @param width Width of the viewport
     * @param height Height of the viewport
     */
    void
    SetViewport(uint32_t x,
                uint32_t y,
                uint32_t width,
                uint32_t height) override;

    /**
     * @brief Sets the clear color
     *
     * @param color Color to use when clearing the buffer
     */
    void
    SetClearColor(const glm::vec4& color) override;

    /**
     * @brief Clears the buffer using a provided single value
     *
     * @param value Value to clear the buffer with
     */
    void
    Clear(uint32_t value) override;

    /**
     * @brief Clears the buffer
     *
     */
    void
    Clear() override;

    /**
     * @brief Renders a mesh buffer
     *
     * @param mesh Mesh buffer to render
     * @param material Material to render with
     * @param camera Camera to use
     * @param modelMatrix Model matrix of the mesh buffer
     */
    void
    RenderIndexed(IMeshBuffer& mesh,
                  IMaterial&   material,
                  ICamera&     camera,
                  glm::mat4    modelMatrix) override;

    /**
     * @brief Applies a compute shader to a frame buffer
     * TODO: Revisit this
     *
     * @param computeShader
     * @param framebuffer
     * @param sourceAttachment
     * @param destinationAttachment
     */
    void
    ApplyComputeShader(std::shared_ptr<IComputeShader> computeShader,
                       std::shared_ptr<IFramebuffer>   framebuffer,
                       uint32_t                        sourceAttachment,
                       uint32_t destinationAttachment) override;

    /**
     * @brief Blits the content of one framebuffer attachment to another
     *
     * @param source Source framebuffer
     * @param destination Destination framebuffer
     * @param sourceAttachment Attachment index to use for the source
     * framebuffer
     * @param destinationAttachment Attachment index to use for the destination
     * framebuffer
     * @param width Width of the attachments
     * @param height Height of the attachments
     */
    void
    Blit(IFramebuffer& source,
         IFramebuffer& destination,
         uint32_t      sourceAttachment,
         uint32_t      destinationAttachment,
         uint32_t      width,
         uint32_t      height) override;

    /**
     * @brief Blits the content of one framebuffer to another using a custom
     * shader
     *
     * @param source Source framebuffer
     * @param destination Destination framebuffer
     * @param sourceAttachment Attachment index to use for the source
     * framebuffer
     * @param destinationAttachmentAttachment index to use for the destination
     * framebuffer
     * @param shader Shader to use for blitting
     */
    void
    CustomBlit(IFramebuffer&              source,
               IFramebuffer&              destination,
               uint32_t                   sourceAttachment,
               uint32_t                   destinationAttachment,
               std::shared_ptr<IMaterial> material,
               bool                       srgb) override;

    /**
     * @brief Returns information about the vram usage
     *
     * @return Struct containing the vram usage data
     */
    auto
    QueryVRAMUsage() -> VRAMUsageBuffer override;

    /**
     * @brief Retrieves the maximum amount of samples for MSAA supprted by the
     * devive
     *
     * @return Amount of samples supported
     */
    auto
    GetMaxSamples() -> int32_t override;
  };
}