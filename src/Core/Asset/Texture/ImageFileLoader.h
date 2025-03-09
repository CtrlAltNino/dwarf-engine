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
    std::shared_ptr<IDwarfLogger>   m_Logger;
    std::shared_ptr<IFileHandler>   m_FileHandler;
    std::shared_ptr<IAssetMetadata> m_AssetMetadata;

  public:
    ImageFileLoader(std::shared_ptr<IDwarfLogger>   logger,
                    std::shared_ptr<IFileHandler>   fileHandler,
                    std::shared_ptr<IAssetMetadata> assetMetadata);
    ~ImageFileLoader() override;
    std::shared_ptr<TextureContainer>
    LoadImageFile(std::filesystem::path texturePath) override;
  };
} // namespace Dwarf