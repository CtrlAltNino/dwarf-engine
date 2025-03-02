#include "TextureLoadingWorker.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "ITextureLoadingWorker.h"
#include <functional>

namespace Dwarf
{
  TextureLoadingWorker::TextureLoadingWorker(
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<IImageFileLoader> imageFileLoader,
    std::shared_ptr<ITextureFactory>  textureFactory)
    : m_Logger(logger)
    , m_ImageFileLoader(imageFileLoader)
    , m_TextureFactory(textureFactory)
  {
    for (int i = 0; i < m_NumWorkerThreads; i++)
    {
      m_TextureWorkers.push_back(
        std::thread([this]() { ProcessTextureLoadRequests(); }));
    }
    m_Logger->LogDebug(
      Log("TextureLoadingWorker created.", "TextureLoadingWorker"));
  }

  TextureLoadingWorker::~TextureLoadingWorker()
  {
    m_Logger->LogDebug(
      Log("Joining Texture Worker Thread", "TextureLoadingWorker"));
    {
      std::lock_guard<std::mutex> lock(m_LoadMutex);
      stopWorker.store(true);
    }
    queueCondition.notify_all();
    for (auto& thread : m_TextureWorkers)
    {
      if (thread.joinable())
      {
        thread.join();
      }
    }
    m_Logger->LogDebug(
      Log("TextureLoadingWorker destroyed.", "TextureLoadingWorker"));
  }

  void
  TextureLoadingWorker::RequestTextureLoad(TextureLoadRequest request)
  {
    m_Logger->LogDebug(
      Log("Adding texture load request", "TextureLadingWorker"));
    {
      std::lock_guard<std::mutex> lock(m_LoadMutex);
      m_TextureLoadRequestQueue.push({ request });
    }
    queueCondition.notify_one(); // Wake up the worker thread
    m_CurrentlyProcessing.insert(request.TexturePath);
  }

  void
  TextureLoadingWorker::RequestTextureUpload(TextureUploadRequest request)
  {
    m_Logger->LogDebug(
      Log("Adding texture upload request", "TextureLadingWorker"));
    {
      std::lock_guard<std::mutex> lock(m_UploadMutex);
      m_TextureUploadRequestQueue.push(request);
    }
  }

  void
  TextureLoadingWorker::ProcessTextureLoadRequests()
  {
    while (!stopWorker.load())
    {
      TextureLoadRequest request;

      { // Lock the queue and wait for work
        std::unique_lock<std::mutex> lock(m_LoadMutex);
        queueCondition.wait(
          lock,
          [this]
          { return !m_TextureLoadRequestQueue.empty() || stopWorker.load(); });

        if (stopWorker.load()) return;

        request = m_TextureLoadRequestQueue.front();
        m_TextureLoadRequestQueue.pop();
      }

      // Load texture from disk (background thread)
      std::shared_ptr<TextureContainer> textureData =
        m_ImageFileLoader->LoadTexture(request.TexturePath);

      if (!textureData)
      {
        m_Logger->LogError(
          Log(fmt::format("Error loading texture from path: {}",
                          request.TexturePath.string()),
              "TextureLoadingWorker"));
        m_CurrentlyProcessing.erase(request.TexturePath);
        continue;
      }

      // Send texture data to the main thread for OpenGL upload
      {
        m_Logger->LogDebug(
          Log("Texture loaded, creating upload job", "TextureLoadingWorker"));
        std::lock_guard<std::mutex> lock(m_UploadMutex);
        m_TextureUploadRequestQueue.push(
          { request.Asset, textureData, request.TexturePath });
      }
    }
  }

  void
  TextureLoadingWorker::ProcessTextureJobs()
  {
    std::lock_guard<std::mutex> lock(m_UploadMutex);
    while (!m_TextureUploadRequestQueue.empty())
    {
      TextureUploadRequest job = m_TextureUploadRequestQueue.front();
      m_TextureUploadRequestQueue.pop();

      job.TextureContainer->Parameters.MinFilter =
        TextureMinFilter::LINEAR_MIPMAP_LINEAR;
      job.TextureContainer->Parameters.MipMapped = true;

      m_Logger->LogInfo(Log("Loading texture async", "TextureLoadingWorker"));

      std::unique_ptr<ITexture> texture =
        m_TextureFactory->FromData(job.TextureContainer);

      job.Asset->SetTexture(std::move(texture));
      m_CurrentlyProcessing.erase(job.TexturePath);
    }
  }

  bool
  TextureLoadingWorker::IsRequested(std::filesystem::path path) const
  {
    return m_CurrentlyProcessing.contains(path);
  }
}