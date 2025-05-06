#pragma once

#include "Core/Rendering/Texture/ITexture.hpp"
#include "Utilities/ImageUtilities/TextureCommon.hpp"
#include <cstdint>
#include <memory>

namespace Dwarf
{
  /**
   * @brief Class that constructs textures on the GPU
   *
   */
  class ITextureFactory
  {
  public:
    virtual ~ITextureFactory() = default;

    /**
     * @brief Creates a texture from a given path
     *
     * @param texturePath Path to a image file
     * @return Unique pointer to the created texture
     */
    [[nodiscard]] virtual auto
    FromPath(std::filesystem::path texturePath) const
      -> std::unique_ptr<ITexture> = 0;

    /**
     * @brief Creates a texture from provided data
     *
     * @param textureData Structure containing data and properties of a texture
     * @return Unique pointer to the created texture
     */
    [[nodiscard]] virtual auto
    FromData(const std::shared_ptr<TextureContainer>& textureData) const
      -> std::unique_ptr<ITexture> = 0;

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
    [[nodiscard]] virtual auto
    Empty(const TextureType&       type,
          const TextureFormat&     format,
          const TextureDataType&   dataType,
          const TextureResolution& size,
          const TextureParameters& parameters,
          int samples = 1) const -> std::unique_ptr<ITexture> = 0;

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
    [[nodiscard]] virtual auto
    Empty(const TextureType&       type,
          const TextureFormat&     format,
          const TextureDataType&   dataType,
          const TextureResolution& size,
          uint32_t samples = 1) const -> std::unique_ptr<ITexture> = 0;

    /**
     * @brief Gets the placeholder texture
     *
     * @return Shared pointer to the placeholder texture
     */
    [[nodiscard]] virtual auto
    GetPlaceholderTexture() -> std::shared_ptr<ITexture> = 0;

    [[nodiscard]] virtual auto
    GenerateCubemapFromEquirectTexture(
      std::shared_ptr<ITexture> equirectangularTexture)
      -> std::shared_ptr<ITexture> = 0;
  };
} // namespace Dwarf