#pragma once

#include "Core/Asset/Database/AssetComponents.h"
#include "Utilities/ImageUtilities/TextureCommon.h"
#include <entt/entity/fwd.hpp>

namespace Dwarf
{
  /**
   * @brief Struct that contains a path to an image file, and a pointer to the
   * texture asset in which the loaded texture will be stored after uploaded to
   * the GPU.
   *
   */
  struct TextureLoadRequest
  {
    TextureAsset*         Asset;
    std::filesystem::path TexturePath;
  };

  /**
   * @brief Struct that contains a path to the loaded image file, a pointer to
   * the texture asset, and the loaded image file
   *
   */
  struct TextureUploadRequest
  {
    TextureAsset*                     Asset;
    std::shared_ptr<TextureContainer> Container;
    std::filesystem::path             TexturePath;
  };

  /**
   * @brief Class that manages texture loading in the form of thread safe
   * requests
   *
   */
  class ITextureLoadingWorker
  {
  public:
    virtual ~ITextureLoadingWorker() = default;

    /**
     * @brief Add a request to load a texture from disk into memory
     *
     * @param request The request to add
     */
    virtual void
    RequestTextureLoad(TextureLoadRequest request) = 0;

    /**
     * @brief Add a request to load a texture from memory into VRAM
     *
     * @param request
     */
    virtual void
    RequestTextureUpload(TextureUploadRequest request) = 0;

    /**
     * @brief Processes texture load requests
     *
     */
    virtual void
    ProcessTextureLoadRequests() = 0;

    /**
     * @brief Processes texture upload requests
     *
     */
    virtual void
    ProcessTextureJobs() = 0;

    /**
     * @brief Checks if a texture is currently on the way from disk to the GPU
     *
     * @param path Path of the image file
     * @return true If the image file has already been requested
     * @return false If the image file is not currently being handled
     */
    virtual auto
    IsRequested(std::filesystem::path path) -> bool = 0;
  };
}