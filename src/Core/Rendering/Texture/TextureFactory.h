#pragma once

#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Core/Base.h"
#include "ITextureFactory.h"
#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Logging/IDwarfLogger.h"
#include <cstdint>

namespace Dwarf
{
  class TextureFactory : public ITextureFactory
  {
  private:
    GraphicsApi                       m_Api;
    std::shared_ptr<IImageFileLoader> m_ImageFileLoader;
    std::shared_ptr<IDwarfLogger>     m_Logger;

    uint64_t
    GetPixelCount(const TextureResolution& size, const TextureType& type);

    uint64_t
    GetBytesPerPixel(const TextureFormat& format, const TextureDataType& type);

    std::shared_ptr<TextureParameters>
    GetParameters(std::filesystem::path const& path);

    std::shared_ptr<ITexture>
    LoadTexture(std::shared_ptr<TextureContainer> textureData);

  public:
    TextureFactory(GraphicsApi                       api,
                   std::shared_ptr<IImageFileLoader> loader,
                   std::shared_ptr<IDwarfLogger>     logger);
    ~TextureFactory() override;
    std::shared_ptr<ITexture>
    FromPath(std::filesystem::path texturePath) override;

    std::shared_ptr<ITexture>
    FromData(const std::shared_ptr<TextureContainer>& textureData) override;

    std::shared_ptr<ITexture>
    Empty(const TextureType&       type,
          const TextureFormat&     format,
          const TextureDataType&   dataType,
          const TextureResolution& size,
          const TextureParameters& parameters,
          int                      samples = 1) override;

    virtual std::shared_ptr<ITexture>
    Empty(const TextureType&       type,
          const TextureFormat&     format,
          const TextureDataType&   dataType,
          const TextureResolution& size,
          int                      samples = 1) override;
  };
} // namespace Dwarf