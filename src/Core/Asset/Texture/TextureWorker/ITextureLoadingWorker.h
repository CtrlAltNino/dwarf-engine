#pragma once

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/UUID.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <entt/entity/fwd.hpp>
namespace Dwarf
{
  struct TextureLoadRequest
  {
    TextureAsset*         Asset;
    std::filesystem::path TexturePath;
  };

  struct TextureUploadRequest
  {
    TextureAsset*                     Asset;
    std::shared_ptr<TextureContainer> TextureContainer;
    std::filesystem::path             TexturePath;
  };

  class ITextureLoadingWorker
  {
  public:
    virtual ~ITextureLoadingWorker() = default;
    virtual void
    RequestTextureLoad(TextureLoadRequest request) = 0;

    virtual void
    RequestTextureUpload(TextureUploadRequest request) = 0;

    virtual void
    ProcessTextureLoadRequests() = 0;

    virtual void
    ProcessTextureJobs() = 0;

    virtual bool
    IsRequested(std::filesystem::path path) const = 0;
  };
}