#pragma once

#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "IImageFileLoader.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"

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
    std::shared_ptr<TextureContainer>
    LoadImageFile(std::filesystem::path texturePath) override;
  };
} // namespace Dwarf