#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.hpp"
#include "Core/Rendering/Material/IMaterial.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.hpp"
#include "Core/Rendering/PingPongBuffer/IPingPongBuffer.hpp"
#include "Core/Rendering/Shader/IComputeShader.hpp"
#include "Core/Scene/Camera/ICamera.hpp"

namespace Dwarf
{
  struct VRAMUsageBuffer
  {
    int totalMemoryMb;
    int usedMemoryMb;
  };

  /**
   * @brief Abstraction layer to interface with the graphics API
   *
   */
  class IRendererApi
  {
  public:
    virtual ~IRendererApi() = default;

    /**
     * @brief Sets the viewport
     *
     * @param x X position of the viewport
     * @param y Y position of the viewport
     * @param width Width of the viewport
     * @param height Height of the viewport
     */
    virtual void
    SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

    /**
     * @brief Sets the clear color
     *
     * @param color Color to use when clearing the buffer
     */
    virtual void
    SetClearColor(const glm::vec4& color) = 0;

    /**
     * @brief Clears the buffer
     *
     */
    virtual void
    Clear() = 0;

    /**
     * @brief Clears the buffer using a provided single value
     *
     * @param value Value to clear the buffer with
     */
    virtual void
    Clear(uint32_t value) = 0;

    /**
     * @brief Renders a mesh buffer
     *
     * @param mesh Mesh buffer to render
     * @param material Material to render with
     * @param camera Camera to use
     * @param modelMatrix Model matrix of the mesh buffer
     */
    virtual void
    RenderIndexed(const IMeshBuffer* mesh,
                  IMaterial&         material,
                  ICamera&           camera,
                  glm::mat4          modelMatrix) = 0;

    virtual void
    RenderSkyboxIndexed(const IMeshBuffer* mesh,
                        IShader&           shader,
                        ICamera&           camera) = 0;

    virtual void
    RenderSkyboxIndexed(const IMeshBuffer* mesh,
                        IMaterial&         material,
                        ICamera&           camera) = 0;

    /**
     * @brief Applies a compute shader to a frame buffer
     * TODO: Revisit this
     *
     * @param computeShader
     * @param framebuffer
     * @param sourceAttachment
     * @param destinationAttachment
     */
    virtual void
    ApplyComputeShader(std::shared_ptr<IComputeShader> computeShader,
                       std::shared_ptr<IFramebuffer>   framebuffer,
                       uint32_t                        sourceAttachment,
                       uint32_t destinationAttachment) = 0;

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
    virtual void
    Blit(IFramebuffer& source,
         IFramebuffer& destination,
         uint32_t      sourceAttachment,
         uint32_t      destinationAttachment,
         uint32_t      width,
         uint32_t      height) = 0;

    /**
     * @brief Blits the depth information of one framebuffer to another
     *
     * @param source Source framebuffer
     * @param destination Destination framebuffer
     * @param width Width of the framebuffers
     * @param height Height of the framebuffers
     */
    virtual void
    BlitDepth(IFramebuffer& source,
              IFramebuffer& destination,
              uint32_t      width,
              uint32_t      height) = 0;

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
     * @param srgb If the buffer is SRGB
     */
    virtual void
    CustomBlit(IFramebuffer& source,
               IFramebuffer& destination,
               uint32_t      sourceAttachment,
               uint32_t      destinationAttachment,
               IShader&      shader,
               bool          srgb) = 0;

    /**
     * @brief Applies a post process shader onto a ping pong buffer
     *
     * @param buffer Ping pong buffer to use
     * @param shader Shader to use
     * @param srgb If the buffer is SRGB
     */
    virtual void
    ApplyPostProcess(IPingPongBuffer& buffer, IShader& shader, bool srgb) = 0;

    /**
     * @brief Returns information about the vram usage
     *
     * @return Struct containing the vram usage data
     */
    virtual auto
    QueryVRAMUsage() -> VRAMUsageBuffer = 0;

    /**
     * @brief Retrieves the maximum amount of samples for MSAA supprted by the
     * devive
     *
     * @return Amount of samples supported
     */
    virtual auto
    GetMaxSamples() -> int32_t = 0;

    virtual auto
    IsAnisoSupported() -> bool = 0;

    virtual auto
    GetMaxAnisoLevel() -> uint8_t = 0;
  };
}