#pragma once

#include "Core/Asset/Texture/IImageFileLoader.hpp"
#include "Core/Rendering/Texture/ITextureFactory.hpp"
#include "ITextureLoadingWorker.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <unordered_set>

namespace Dwarf
{
  class TextureLoadingWorker : public ITextureLoadingWorker
  {
  private:
    std::shared_ptr<IDwarfLogger>     mLogger;
    std::shared_ptr<IImageFileLoader> mImageFileLoader;
    std::shared_ptr<ITextureFactory>  mTextureFactory;

    // Queue for loading requests per thread
    std::mutex                     mLoadingMutex;
    std::queue<TextureLoadRequest> mTextureLoadRequestQueue;

    // Queue for uploading to the gpu
    std::mutex mUploadMutex;
    std::queue<std::unique_ptr<TextureUploadRequest>>
      mTextureUploadRequestQueue;

    // Condition for waiting
    std::condition_variable queueCondition;

    // Flag to stop the threads
    std::atomic<bool> stopWorker = false;

    // Worker threads for loading textures into memory
    std::vector<std::thread> mTextureWorkers;

    // number of threads
    uint32_t mNumWorkerThreads = 1;

    // Keeping track of which textures are currently being processed
    std::mutex                                mCurrentlyProcessingMutex;
    std::unordered_set<std::filesystem::path> mCurrentlyProcessing;

  public:
    TextureLoadingWorker(std::shared_ptr<IDwarfLogger>     logger,
                         std::shared_ptr<IImageFileLoader> imageFileLoader,
                         std::shared_ptr<ITextureFactory>  textureFactory);

    ~TextureLoadingWorker() override;

    /**
     * @brief Add a request to load a texture from disk into memory
     *
     * @param request The request to add
     */
    void
    RequestTextureLoad(TextureLoadRequest request) override;

    /**
     * @brief Add a request to load a texture from memory into VRAM
     *
     * @param request
     */
    void
    RequestTextureUpload(TextureUploadRequest request) override;

    /**
     * @brief Processes texture load requests
     *
     */
    void
    ProcessTextureLoadRequests() override;

    /**
     * @brief Processes texture upload requests
     *
     */
    void
    ProcessTextureJobs() override;

    /**
     * @brief Checks if a texture is currently on the way from disk to the GPU
     *
     * @param path Path of the image file
     * @return true If the image file has already been requested
     * @return false If the image file is not currently being handled
     */
    auto
    IsRequested(std::filesystem::path path) -> bool override;
  };
}