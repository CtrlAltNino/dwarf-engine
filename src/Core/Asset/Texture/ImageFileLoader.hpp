#pragma once

#include "Core/Asset/Metadata/IAssetMetadata.hpp"
#include "IImageFileLoader.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "Utilities/FileHandler/IFileHandler.hpp"

namespace Dwarf
{
  class ImageFileLoader : public IImageFileLoader
  {
  private:
    std::shared_ptr<IDwarfLogger>   mLogger;
    std::shared_ptr<IFileHandler>   mFileHandler;
    std::shared_ptr<IAssetMetadata> mAssetMetadata;

  public:
    ImageFileLoader(std::shared_ptr<IDwarfLogger>   logger,
                    std::shared_ptr<IFileHandler>   fileHandler,
                    std::shared_ptr<IAssetMetadata> assetMetadata);
    ~ImageFileLoader() override;

    /**
     * @brief Loads and decodes an image file from disk into memory
     *
     * @param imagePath Path to the imagel file
     * @return The container storing the image data and properties
     */
    auto
    LoadImageFile(std::filesystem::path imagePath)
      -> std::shared_ptr<TextureContainer> override;
  };
} // namespace Dwarf