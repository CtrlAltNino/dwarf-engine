#pragma once

#include "IImageFileLoader.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"

namespace Dwarf
{
  class ImageFileLoader : public IImageFileLoader
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    std::shared_ptr<IFileHandler> m_FileHandler;

  public:
    ImageFileLoader(std::shared_ptr<IDwarfLogger> logger,
                    std::shared_ptr<IFileHandler> fileHandler);
    ~ImageFileLoader() override;
    std::shared_ptr<TextureContainer>
    LoadTexture(std::filesystem::path texturePath) override;
  };
} // namespace Dwarf