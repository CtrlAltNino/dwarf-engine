#include "pch.hpp"

#include "Core/Asset/Database/AssetComponents.hpp"
#include "Core/Asset/Texture/IImageFileLoader.hpp"
#include "Core/Rendering/Texture/ITextureFactory.hpp"
#include "ITextureLoadingWorker.hpp"
#include "TextureLoadingWorker.hpp"

namespace Dwarf
{
  TextureLoadingWorker::TextureLoadingWorker(
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<IImageFileLoader> imageFileLoader,
    std::shared_ptr<ITextureFactory>  textureFactory)
    : mLogger(std::move(logger))
    , mImageFileLoader(std::move(imageFileLoader))
    , mTextureFactory(std::move(textureFactory))
    , mNumWorkerThreads(std::thread::hardware_concurrency() - 1)
  {
    for (int i = 0; i < mNumWorkerThreads; i++)
    {
      mTextureWorkers.emplace_back([this]() { ProcessTextureLoadRequests(); });
    }
    mLogger->LogDebug(
      Log("TextureLoadingWorker created.", "TextureLoadingWorker"));
  }

  TextureLoadingWorker::~TextureLoadingWorker()
  {
    mLogger->LogDebug(
      Log("Joining Texture Worker Thread", "TextureLoadingWorker"));
    stopWorker.store(true);
    queueCondition.notify_all();
    for (auto& thread : mTextureWorkers)
    {
      if (thread.joinable())
      {
        thread.join();
      }
    }
    mLogger->LogDebug(
      Log("TextureLoadingWorker destroyed.", "TextureLoadingWorker"));
  }

  void
  TextureLoadingWorker::RequestTextureLoad(TextureLoadRequest request)
  {
    std::unique_ptr<TextureLoadRequest> requestPtr = nullptr;
    {
      {
        std::unique_lock<std::mutex> lock(mLoadingMutex);
        mTextureLoadRequestQueue.push(request);
      }
      {
        std::unique_lock<std::mutex> lock(mCurrentlyProcessingMutex);
        mCurrentlyProcessing.insert(request.TexturePath);
      }
      mLogger->LogDebug(
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
      std::lock_guard<std::mutex> lock(mUploadMutex);
      mTextureUploadRequestQueue.push(std::move(requestPtr));
      mLogger->LogDebug(
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
        std::unique_lock<std::mutex> lock(mLoadingMutex);
        queueCondition.wait(lock,
                            [this] {
                              return !mTextureLoadRequestQueue.empty() ||
                                     stopWorker.load();
                            });

        if (stopWorker.load())
        {
          return;
        }

        request = mTextureLoadRequestQueue.front();
        mTextureLoadRequestQueue.pop();
      }
      // Load texture from disk (background thread)
      std::shared_ptr<TextureContainer> textureData =
        mImageFileLoader->LoadImageFile(request.TexturePath);

      if (!textureData)
      {
        mLogger->LogError(Log(fmt::format("Error loading texture from path: {}",
                                          request.TexturePath.string()),
                              "TextureLoadingWorker"));
        {
          std::unique_lock<std::mutex> lock(mCurrentlyProcessingMutex);
          mCurrentlyProcessing.erase(request.TexturePath);
        }
        continue;
      }

      // Send texture data to the main thread for OpenGL upload
      {
        std::lock_guard<std::mutex>           lock(mUploadMutex);
        std::unique_ptr<TextureUploadRequest> ptr =
          std::make_unique<TextureUploadRequest>(
            request.Asset, textureData, request.TexturePath);
        mTextureUploadRequestQueue.push(std::move(ptr));
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
        std::unique_lock<std::mutex> lock(mUploadMutex);

        if (mTextureUploadRequestQueue.empty())
        {
          return;
        }
        job = std::move(mTextureUploadRequestQueue.front());
        mTextureUploadRequestQueue.pop();
        {
          std::unique_lock<std::mutex> lock(mCurrentlyProcessingMutex);
          mCurrentlyProcessing.erase(job->TexturePath);
        }
      }

      mLogger->LogInfo(
        Log("Uploading texture into GPU", "TextureLoadingWorker"));

      std::unique_ptr<ITexture> texture =
        mTextureFactory->FromData(job->Container);

      job->Asset->SetTexture(std::move(texture));
    }
  }

  bool
  TextureLoadingWorker::IsRequested(std::filesystem::path path)
  {
    std::unique_lock<std::mutex> lock(mCurrentlyProcessingMutex);
    return mCurrentlyProcessing.contains(path);
  }
}