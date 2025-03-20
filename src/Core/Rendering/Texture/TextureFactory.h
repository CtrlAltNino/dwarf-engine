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
    std::shared_ptr<ITexture>         mPlaceholderTexture;

    uint64_t
    GetPixelCount(const TextureResolution& size, const TextureType& type);

    uint64_t
    GetBytesPerPixel(const TextureFormat& format, const TextureDataType& type);

    std::shared_ptr<TextureParameters>
    GetParameters(std::filesystem::path const& path);

    std::unique_ptr<ITexture>
    LoadTexture(std::shared_ptr<TextureContainer> textureData);

  public:
    TextureFactory(GraphicsApi                       api,
                   std::shared_ptr<IImageFileLoader> loader,
                   std::shared_ptr<IDwarfLogger>     logger,
                   std::shared_ptr<IVramTracker>     vramTracker);
    ~TextureFactory() override;
    std::unique_ptr<ITexture>
    FromPath(std::filesystem::path texturePath) override;

    std::unique_ptr<ITexture>
    FromData(const std::shared_ptr<TextureContainer>& textureData) override;

    std::unique_ptr<ITexture>
    Empty(const TextureType&       type,
          const TextureFormat&     format,
          const TextureDataType&   dataType,
          const TextureResolution& size,
          const TextureParameters& parameters,
          int                      samples = 1) override;

    std::unique_ptr<ITexture>
    Empty(const TextureType&       type,
          const TextureFormat&     format,
          const TextureDataType&   dataType,
          const TextureResolution& size,
          int                      samples = 1) override;

    std::shared_ptr<ITexture>
    GetPlaceholderTexture() override;
  };
} // namespace Dwarf