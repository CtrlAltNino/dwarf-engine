#include "TextureLoadingWorker.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Texture/IImageFileLoader.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "ITextureLoadingWorker.h"

namespace Dwarf
{
  TextureLoadingWorker::TextureLoadingWorker(
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<IImageFileLoader> imageFileLoader,
    std::shared_ptr<ITextureFactory>  textureFactory,
    std::shared_ptr<IAssetDatabase>   assetDatabase)
    : m_Logger(logger)
    , m_ImageFileLoader(imageFileLoader)
    , m_TextureFactory(textureFactory)
    , m_AssetDatabase(assetDatabase)
  {
  }

  void
  TextureLoadingWorker::RequestTextureLoad(TextureLoadRequest request)
  {
    {
      std::lock_guard<std::mutex> lock(m_LoadMutex);
      m_TextureLoadRequestQueue.push({ request });
    }
    queueCondition.notify_one(); // Wake up the worker thread
  }

  void
  TextureLoadingWorker::RequestTextureUpload(TextureUploadRequest request)
  {
    {
      std::lock_guard<std::mutex> lock(m_UploadMutex);
      m_TextureUploadRequestQueue.push(request);
    }
  }

  void
  TextureLoadingWorker::ProcessTextureLoadRequests()
  {
    while (true)
    {
      TextureLoadRequest request;

      { // Lock the queue and wait for work
        std::unique_lock<std::mutex> lock(m_LoadMutex);
        queueCondition.wait(
          lock,
          [this] { return !m_TextureLoadRequestQueue.empty() || stopWorker; });

        if (stopWorker && m_TextureLoadRequestQueue.empty()) return;

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
        continue;
      }

      // Send texture data to the main thread for OpenGL upload
      {
        std::lock_guard<std::mutex> lock(m_UploadMutex);
        m_TextureUploadRequestQueue.push({ request.Id, textureData });
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

      std::unique_ptr<ITexture> texture =
        m_TextureFactory->FromData(job.TextureContainer);

      std::unique_ptr<IAssetReference> ref = m_AssetDatabase->Retrieve(job.Id);

      TextureAsset& asset = (TextureAsset&)ref->GetAsset();
      asset.SetTexture(std::move(texture));
    }
  }
}