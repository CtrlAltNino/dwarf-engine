#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "ITextureLoadingWorker.h"
#include "Logging/IDwarfLogger.h"
#include <mutex>
#include <queue>
#include <condition_variable>
#include <unordered_set>

namespace Dwarf
{
  class TextureLoadingWorker : public ITextureLoadingWorker
  {
  private:
    std::shared_ptr<IDwarfLogger>     m_Logger;
    std::shared_ptr<IImageFileLoader> m_ImageFileLoader;
    std::shared_ptr<ITextureFactory>  m_TextureFactory;

    std::queue<TextureLoadRequest> m_TextureLoadRequestQueue;
    std::mutex                     m_LoadMutex;

    std::queue<TextureUploadRequest> m_TextureUploadRequestQueue;
    std::mutex                       m_UploadMutex;

    std::condition_variable queueCondition;
    std::atomic<bool>       stopWorker = false;

    std::vector<std::thread> m_TextureWorkers;

    int m_NumWorkerThreads = 4;

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
    IsRequested(std::filesystem::path path) const override;
  };
}