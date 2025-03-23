#pragma once

#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Core/Base.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "ITextureFactory.h"
#include "Logging/IDwarfLogger.h"
#include <cstdint>

namespace Dwarf
{
  class TextureFactory : public ITextureFactory
  {
  private:
    GraphicsApi                       mApi;
    std::shared_ptr<IImageFileLoader> mImageFileLoader;
    std::shared_ptr<IDwarfLogger>     mLogger;
    std::shared_ptr<IVramTracker>     mVramTracker;
    std::unique_ptr<ITexture>         mPlaceholderTexture;

    /**
     * @brief Helper function that calculates the pixel count of a texture
     *
     * @param size Resolution of the texture
     * @param type Dimensions of the texture
     * @return Calculated amount of texture
     */
    static auto
    GetPixelCount(const TextureResolution& size, const TextureType& type)
      -> uint64_t;

    /**
     * @brief Calculates the amount of bytes per pixel
     *
     * @param format Format of the texture
     * @param type Data type of the texture
     * @return Calculated amount of bytes per pixel
     */
    static auto
    GetBytesPerPixel(const TextureFormat& format, const TextureDataType& type)
      -> uint64_t;

    /**
     * @brief Loads the texture data into GPU
     *
     * @param textureData Structure containing the texture data
     * @return Unique pointer to the created GPU texture
     */
    [[nodiscard]] auto
    LoadTexture(const std::shared_ptr<TextureContainer>& textureData) const
      -> std::unique_ptr<ITexture>;

  public:
    TextureFactory(GraphicsApi                       api,
                   std::shared_ptr<IImageFileLoader> loader,
                   std::shared_ptr<IDwarfLogger>     logger,
                   std::shared_ptr<IVramTracker>     vramTracker);
    ~TextureFactory() override;

    /**
     * @brief Creates a texture from a given path
     *
     * @param texturePath Path to a image file
     * @return Unique pointer to the created texture
     */
    [[nodiscard]] auto
    FromPath(std::filesystem::path texturePath) const
      -> std::unique_ptr<ITexture> override;

    /**
     * @brief Creates a texture from provided data
     *
     * @param textureData Structure containing data and properties of a texture
     * @return Unique pointer to the created texture
     */
    [[nodiscard]] auto
    FromData(const std::shared_ptr<TextureContainer>& textureData) const
      -> std::unique_ptr<ITexture> override;

    /**
     * @brief Creates an empty texture with given properties
     *
     * @param type Dimension of the texture
     * @param format Format of the texture
     * @param dataType Data type of the texture
     * @param size Size of the texture
     * @param parameters Parameters for the texture
     * @param samples Amount of samples if the texture should be multi sampled
     * @return Unique pointer to the created texture
     */
    [[nodiscard]] auto
    Empty(const TextureType&       type,
          const TextureFormat&     format,
          const TextureDataType&   dataType,
          const TextureResolution& size,
          const TextureParameters& parameters,
          int samples = 1) const -> std::unique_ptr<ITexture> override;

    /**
     * @brief Creates an empty texture with given properties
     *
     * @param type Dimension of the texture
     * @param format Format of the texture
     * @param dataType Data type of the texture
     * @param size Size of the texture
     * @param samples Amount of samples if the texture should be multi sampled
     * @return Unique pointer to the created texture
     */
    [[nodiscard]] auto
    Empty(const TextureType&       type,
          const TextureFormat&     format,
          const TextureDataType&   dataType,
          const TextureResolution& size,
          uint32_t samples = 1) const -> std::unique_ptr<ITexture> override;

    /**
     * @brief Gets the placeholder texture
     *
     * @return Shared pointer to the placeholder texture
     */
    [[nodiscard]] auto
    GetPlaceholderTexture() -> ITexture& override;
  };
} // namespace Dwarf