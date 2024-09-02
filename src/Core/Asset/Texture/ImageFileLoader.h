#pragma once

#include "IImageFileLoader.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class ImageFileLoader : public IImageFileLoader
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;

  public:
    ImageFileLoader(std::shared_ptr<IDwarfLogger> logger);
    std::shared_ptr<TextureContainer>
    LoadTexture(std::filesystem::path texturePath) override;
  };
} // namespace Dwarf