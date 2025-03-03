#pragma once

#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "ITextureLoadingWorker.h"
#include "Logging/IDwarfLogger.h"
#include <mutex>
#include <queue>
#include <condition_variable>
#include <unordered_set>
#include <boost/lockfree/queue.hpp>

namespace Dwarf
{
  class TextureLoadingWorker : public ITextureLoadingWorker
  {
  private:
    std::shared_ptr<IDwarfLogger>     m_Logger;
    std::shared_ptr<IImageFileLoader> m_ImageFileLoader;
    std::shared_ptr<ITextureFactory>  m_TextureFactory;

    // Queue for loading requests per thread
    std::mutex                     m_LoadingMutex;
    std::queue<TextureLoadRequest> m_TextureLoadRequestQueue;

    // Queue for uploading to the gpu
    std::mutex m_UploadMutex;
    std::queue<std::unique_ptr<TextureUploadRequest>>
      m_TextureUploadRequestQueue;

    // Condition for waiting
    std::condition_variable queueCondition;

    // Flag to stop the threads
    std::atomic<bool> stopWorker = false;

    // Worker threads for loading textures into memory
    std::vector<std::thread> m_TextureWorkers;

    // number of threads
    int m_NumWorkerThreads = 1;

    // Keeping track of which textures are currently being processed
    std::mutex                                m_CurrentlyProcessingMutex;
    std::unordered_set<std::filesystem::path> m_CurrentlyProcessing;

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