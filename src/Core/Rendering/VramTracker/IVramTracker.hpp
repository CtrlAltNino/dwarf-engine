#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.hpp"
#include "Utilities/ImageUtilities/TextureCommon.hpp"
#include <cstddef>

namespace Dwarf
{
  /**
   * @brief Class that manually tracks the amount of VRAM that is being used by
   * the application. Furthermore it distincts between types of memory
   * allocations.
   *
   */
  class IVramTracker
  {
  public:
    virtual ~IVramTracker() = default;

    /**
     * @brief Adds VRAM in bytes
     *
     * @param bytes Amount of VRAM added
     */
    virtual void
    AddTextureMemory(size_t bytes) = 0;

    /**
     * @brief Calculates and adds the VRAM amount from the given texture
     * properties
     *
     * @param format Format of the texture
     * @param dataType Data type of the texture
     * @param size Size of the texture
     * @param samples Samples of the texture
     * @return Amount of VRAM added
     */
    virtual auto
    AddTextureMemory(const TextureFormat&     format,
                     const TextureDataType&   dataType,
                     const TextureResolution& size,
                     int                      samples) -> size_t = 0;

    /**
     * @brief Calculates and adds the VRAM amount from the given texture
     *
     * @param texture Texture container to calculate the VRAM amount from
     * @return Amount of VRAM added
     */
    virtual auto
    AddTextureMemory(std::shared_ptr<TextureContainer> texture) -> size_t = 0;

    /**
     * @brief Removes the given VRAM amount
     *
     * @param bytes Amount of VRAM to remove in bytes
     */
    virtual void
    RemoveTextureMemory(size_t bytes) = 0;

    /**
     * @brief Calculates and removes the amount of VRAM from the given texture
     * properties
     *
     * @param type Dimensions of the texture
     * @param format Format of the texture
     * @param dataType Data type of the texture
     * @param size Size of the texture
     * @param samples Samples of the texture
     */
    virtual void
    RemoveTextureMemory(const TextureFormat&     format,
                        const TextureDataType&   dataType,
                        const TextureResolution& size,
                        int                      samples) = 0;

    /**
     * @brief Calculates and removes the amount of VRAM from the given texture
     *
     * @param texture Texture to calculate the VRAM from
     */
    virtual void
    RemoveTextureMemory(std::shared_ptr<TextureContainer> texture) = 0;

    /**
     * @brief Gets the amount of VRAM textures are using
     *
     * @return size_t
     */
    [[nodiscard]] virtual auto
    GetTextureMemory() const -> size_t = 0;

    /**
     * @brief Adds VRAM used by buffers in bytes
     *
     * @param bytes The amount of VRAM to add
     */
    virtual void
    AddBufferMemory(size_t bytes) = 0;

    /**
     * @brief Removes VRAM used by buffers
     *
     * @param bytes Amount of VRAM to remove in bytes
     */
    virtual void
    RemoveBufferMemory(size_t bytes) = 0;

    /**
     * @brief Gets the amount of VRAM buffers are using
     *
     * @return Amount of VRAM in bytes
     */
    [[nodiscard]] virtual auto
    GetBufferMemory() const -> size_t = 0;

    /**
     * @brief Adds VRAM used by framebuffers
     *
     * @param bytes Amount of VRAM to add in bytes
     */
    virtual void
    AddFramebufferMemory(size_t bytes) = 0;

    /**
     * @brief Calculates and adds framebuffer VRAM based on the given
     * specification
     *
     * @param specification Framebuffer specification
     * @return Amount of VRAM added in bytes
     */
    virtual auto
    AddFramebufferMemory(FramebufferSpecification specification) -> size_t = 0;

    /**
     * @brief Remove framebuffer VRAM
     *
     * @param bytes Amount of VRAM to remove
     */
    virtual void
    RemoveFramebufferMemory(size_t bytes) = 0;

    /**
     * @brief Calculate and remove framebuffer VRAM based on the given
     * specification
     *
     * @param specification Framebuffer specification
     */
    virtual void
    RemoveFramebufferMemory(FramebufferSpecification specification) = 0;

    /**
     * @brief Gets the amount of VRAM framebuffers are using
     *
     * @return Amount of VRAM in bytes
     */
    [[nodiscard]] virtual auto
    GetFramebufferMemory() const -> size_t = 0;

    /**
     * @brief Adds VRAM used by shader programs
     *
     * @param bytes Amount of VRAM in bytes
     */
    virtual void
    AddShaderMemory(size_t bytes) = 0;

    /**
     * @brief Removes VRAM used by shader programs
     *
     * @param bytes Amount of VRAM in bytes
     */
    virtual void
    RemoveShaderMemory(size_t bytes) = 0;

    /**
     * @brief Gets the amount of VRAM used by shaders
     *
     * @return Amount of VRAM in bytes
     */
    [[nodiscard]] virtual auto
    GetShaderMemory() const -> size_t = 0;

    /**
     * @brief Adds VRAM used by compute shader programs
     *
     * @param bytes Amount of VRAM in bytes
     */
    virtual void
    AddComputeMemory(size_t bytes) = 0;

    /**
     * @brief Removes VRAM used by compute shader programs
     *
     * @param bytes Amount of VRAM in bytes
     */
    virtual void
    RemoveComputeMemory(size_t bytes) = 0;

    /**
     * @brief Gets the amount of VRAM used by compute shaders
     *
     * @return Amount of VRAM in bytes
     */
    [[nodiscard]] virtual auto
    GetComputeMemory() const -> size_t = 0;

    /**
     * @brief Get the total amount of VRAM used by the application
     *
     * @return Amount of VRAM in bytes
     */
    [[nodiscard]] virtual auto
    GetTotalMemory() const -> size_t = 0;
  };
}