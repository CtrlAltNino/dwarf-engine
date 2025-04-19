#pragma once

#include "Utilities/ImageUtilities/TextureCommon.hpp"

namespace Dwarf
{
  class IImageFileLoader
  {
  public:
    virtual ~IImageFileLoader() = default;

    /**
     * @brief Loads and decodes an image file from disk into memory
     *
     * @param imagePath Path to the imagel file
     * @return The container storing the image data and properties
     */
    virtual auto
    LoadImageFile(std::filesystem::path imagePath)
      -> std::shared_ptr<TextureContainer> = 0;
  };
} // namespace Dwarf