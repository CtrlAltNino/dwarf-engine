#pragma once

#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "ITextureLoadingWorker.h"
#include "Logging/IDwarfLogger.h"
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
    int mNumWorkerThreads = 1;

    // Keeping track of which textures are currently being processed
    std::mutex                                mCurrentlyProcessingMutex;
    std::unordered_set<std::filesystem::path> mCurrentlyProcessing;

  public:
    TextureLoadingWorker(std::shared_ptr<IDwarfLogger>     logger,
                         std::shared_ptr<IImageFileLoader> imageFileLoader,
                         std::shared_ptr<ITextureFactory>  textureFactory);

    ~TextureLoadingWorker() override;

    void
    RequestTextureLoad(TextureLoadRequest request) override;

    void
    RequestTextureUpload(TextureUploadRequest request) override;

    void
    ProcessTextureLoadRequests() override;

    void
    ProcessTextureJobs() override;

    bool
    IsRequested(std::filesystem::path path) override;
  };
}