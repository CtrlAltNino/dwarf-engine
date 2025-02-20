#pragma once

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/UUID.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <entt/entity/fwd.hpp>
namespace Dwarf
{
  struct TextureLoadRequest
  {
    std::weak_ptr<TextureAsset> Asset;
    std::filesystem::path       TexturePath;
  };

  struct TextureUploadRequest
  {
    std::weak_ptr<TextureAsset>       Asset;
    std::shared_ptr<TextureContainer> TextureContainer;
  };

  class ITextureLoadingWorker
  {
  public:
    virtual void
    RequestTextureLoad(TextureLoadRequest request) = 0;

    virtual void
    RequestTextureUpload(TextureUploadRequest request) = 0;

    virtual void
    ProcessTextureLoadRequests() = 0;

    virtual void
    ProcessTextureJobs() = 0;
  };
}