#pragma once

#include "IVramTracker.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class VramTracker : public IVramTracker
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    size_t                        mTextureMemory = 0;
    size_t                        mBufferMemory = 0;
    size_t                        mFramebufferMemory = 0;
    size_t                        mShaderMemory = 0;
    size_t                        mComputeMemory = 0;

  public:
    VramTracker(std::shared_ptr<IDwarfLogger> logger);
    ~VramTracker() override;

    /**
     * @brief Adds VRAM in bytes
     *
     * @param bytes Amount of VRAM added
     */
    void
    AddTextureMemory(size_t bytes) override;

    /**
     * @brief Calculates and adds the VRAM amount from the given texture
     * properties
     *
     * @param type Dimensions of the texture
     * @param format Format of the texture
     * @param dataType Data type of the texture
     * @param size Size of the texture
     * @param samples Samples of the texture
     * @return Amount of VRAM added
     */
    auto
    AddTextureMemory(const TextureFormat&     format,
                     const TextureDataType&   dataType,
                     const TextureResolution& size,
                     int                      samples) -> size_t override;

    /**
     * @brief Calculates and adds the VRAM amount from the given texture
     *
     * @param texture Texture container to calculate the VRAM amount from
     * @return Amount of VRAM added
     */
    auto
    AddTextureMemory(std::shared_ptr<TextureContainer> texture)
      -> size_t override;

    /**
     * @brief Removes the given VRAM amount
     *
     * @param bytes Amount of VRAM to remove in bytes
     */
    void
    RemoveTextureMemory(size_t bytes) override;

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
    void
    RemoveTextureMemory(const TextureFormat&     format,
                        const TextureDataType&   dataType,
                        const TextureResolution& size,
                        int                      samples) override;

    /**
     * @brief Calculates and removes the amount of VRAM from the given texture
     *
     * @param texture Texture to calculate the VRAM from
     */
    void
    RemoveTextureMemory(std::shared_ptr<TextureContainer> texture) override;

    /**
     * @brief Gets the amount of VRAM textures are using
     *
     * @return size_t
     */
    [[nodiscard]] auto
    GetTextureMemory() const -> size_t override;

    /**
     * @brief Adds VRAM used by buffers in bytes
     *
     * @param bytes The amount of VRAM to add
     */
    void
    AddBufferMemory(size_t bytes) override;

    /**
     * @brief Removes VRAM used by buffers
     *
     * @param bytes Amount of VRAM to remove in bytes
     */
    void
    RemoveBufferMemory(size_t bytes) override;

    /**
     * @brief Gets the amount of VRAM buffers are using
     *
     * @return Amount of VRAM in bytes
     */
    [[nodiscard]] auto
    GetBufferMemory() const -> size_t override;

    /**
     * @brief Adds VRAM used by framebuffers
     *
     * @param bytes Amount of VRAM to add in bytes
     */
    void
    AddFramebufferMemory(size_t bytes) override;

    /**
     * @brief Calculates and adds framebuffer VRAM based on the given
     * specification
     *
     * @param specification Framebuffer specification
     * @return Amount of VRAM added in bytes
     */
    auto
    AddFramebufferMemory(FramebufferSpecification specification)
      -> size_t override;

    /**
     * @brief Remove framebuffer VRAM
     *
     * @param bytes Amount of VRAM to remove
     */
    void
    RemoveFramebufferMemory(size_t bytes) override;

    /**
     * @brief Calculate and remove framebuffer VRAM based on the given
     * specification
     *
     * @param specification Framebuffer specification
     */
    void
    RemoveFramebufferMemory(FramebufferSpecification specification) override;

    /**
     * @brief Gets the amount of VRAM framebuffers are using
     *
     * @return Amount of VRAM in bytes
     */
    [[nodiscard]] auto
    GetFramebufferMemory() const -> size_t override;

    /**
     * @brief Adds VRAM used by shader programs
     *
     * @param bytes Amount of VRAM in bytes
     */
    void
    AddShaderMemory(size_t bytes) override;

    /**
     * @brief Removes VRAM used by shader programs
     *
     * @param bytes Amount of VRAM in bytes
     */
    void
    RemoveShaderMemory(size_t bytes) override;

    /**
     * @brief Gets the amount of VRAM used by shaders
     *
     * @return Amount of VRAM in bytes
     */
    [[nodiscard]] auto
    GetShaderMemory() const -> size_t override;

    /**
     * @brief Adds VRAM used by compute shader programs
     *
     * @param bytes Amount of VRAM in bytes
     */
    void
    AddComputeMemory(size_t bytes) override;

    /**
     * @brief Removes VRAM used by compute shader programs
     *
     * @param bytes Amount of VRAM in bytes
     */
    void
    RemoveComputeMemory(size_t bytes) override;

    /**
     * @brief Gets the amount of VRAM used by compute shaders
     *
     * @return Amount of VRAM in bytes
     */
    [[nodiscard]] auto
    GetComputeMemory() const -> size_t override;

    /**
     * @brief Get the total amount of VRAM used by the application
     *
     * @return Amount of VRAM in bytes
     */
    [[nodiscard]] auto
    GetTotalMemory() const -> size_t override;
  };
}