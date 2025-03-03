#include "TextureLoadingWorker.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "ITextureLoadingWorker.h"
#include <glm/common.hpp>
#include <memory>
#include <mutex>
#include <thread>

namespace Dwarf
{
  TextureLoadingWorker::TextureLoadingWorker(
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<IImageFileLoader> imageFileLoader,
    std::shared_ptr<ITextureFactory>  textureFactory)
    : m_Logger(logger)
    , m_ImageFileLoader(imageFileLoader)
    , m_TextureFactory(textureFactory)
    , m_NumWorkerThreads(std::thread::hardware_concurrency() - 1)
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
    stopWorker.store(true);
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
    std::unique_ptr<TextureLoadRequest> requestPtr = nullptr;
    {
      {
        std::unique_lock<std::mutex> lock(m_LoadingMutex);
        m_TextureLoadRequestQueue.push(request);
      }
      {
        std::unique_lock<std::mutex> lock(m_CurrentlyProcessingMutex);
        m_CurrentlyProcessing.insert(request.TexturePath);
      }
      m_Logger->LogDebug(
        Log("Added new Texture load request", "TextureLoadingWorker"));
    }
    queueCondition.notify_one(); // Wake up the worker thread
  }

  void
  TextureLoadingWorker::RequestTextureUpload(TextureUploadRequest request)
  {
    std::unique_ptr<TextureUploadRequest> requestPtr =
      std::make_unique<TextureUploadRequest>(
        request.Asset, request.Container, request.TexturePath);
    {
      std::lock_guard<std::mutex> lock(m_UploadMutex);
      m_TextureUploadRequestQueue.push(std::move(requestPtr));
      m_Logger->LogDebug(
        Log("Added new Texture Upload request", "TextureLoadingWorker"));
    }
  }

  void
  TextureLoadingWorker::ProcessTextureLoadRequests()
  {
    while (!stopWorker.load())
    {
      TextureLoadRequest request;

      { // Lock the queue and wait for work
        std::unique_lock<std::mutex> lock(m_LoadingMutex);
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
        {
          std::unique_lock<std::mutex> lock(m_CurrentlyProcessingMutex);
          m_CurrentlyProcessing.erase(request.TexturePath);
        }
        continue;
      }

      // Send texture data to the main thread for OpenGL upload
      {
        std::lock_guard<std::mutex>           lock(m_UploadMutex);
        std::unique_ptr<TextureUploadRequest> ptr =
          std::make_unique<TextureUploadRequest>(
            request.Asset, textureData, request.TexturePath);
        m_TextureUploadRequestQueue.push(std::move(ptr));
      }
    }
  }

  void
  TextureLoadingWorker::ProcessTextureJobs()
  {
    bool done = false;

    while (!done)
    {
      std::unique_ptr<TextureUploadRequest> job;

      {
        std::unique_lock<std::mutex> lock(m_UploadMutex);

        if (m_TextureUploadRequestQueue.empty()) return;
        job = std::move(m_TextureUploadRequestQueue.front());
        m_TextureUploadRequestQueue.pop();
        {
          std::unique_lock<std::mutex> lock(m_CurrentlyProcessingMutex);
          m_CurrentlyProcessing.erase(job->TexturePath);
        }
      }

      job->Container->Parameters.MinFilter =
        TextureMinFilter::LINEAR_MIPMAP_LINEAR;
      job->Container->Parameters.MipMapped = true;

      m_Logger->LogInfo(
        Log("Uploading texture into GPU", "TextureLoadingWorker"));

      std::unique_ptr<ITexture> texture =
        m_TextureFactory->FromData(job->Container);

      job->Asset->SetTexture(std::move(texture));
    }
  }

  bool
  TextureLoadingWorker::IsRequested(std::filesystem::path path)
  {
    std::unique_lock<std::mutex> lock(m_CurrentlyProcessingMutex);
    return m_CurrentlyProcessing.contains(path);
  }
}