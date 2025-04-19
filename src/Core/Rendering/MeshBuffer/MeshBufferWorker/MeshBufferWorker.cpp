#include "pch.hpp"

#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "IMeshBufferWorker.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "MeshBufferWorker.hpp"

namespace Dwarf
{
  MeshBufferWorker::MeshBufferWorker(
    std::shared_ptr<IDwarfLogger>       logger,
    std::shared_ptr<IMeshBufferFactory> meshBufferFactory)
    : mLogger(std::move(logger))
    , mMeshBufferFactory(std::move(meshBufferFactory))
  {
    mLogger->LogDebug(Log("MeshBufferWorker created.", "MeshBufferWorker"));
  }

  MeshBufferWorker::~MeshBufferWorker()
  {
    mLogger->LogDebug(Log("MeshBufferWorker destroyed.", "MeshBufferWorker"));
  }

  void
  MeshBufferWorker::RequestMeshBuffer(
    std::unique_ptr<MeshBufferRequest>&& request)
  {
    mLogger->LogDebug(Log("Adding mesh buffer request", "MeshBufferWorker"));
    std::lock_guard<std::mutex> lock(mUploadMutex);
    mMeshBufferRequestQueue.push(std::move(request));
  }

  void
  MeshBufferWorker::ProcessRequests()
  {
    std::lock_guard<std::mutex> lock(mUploadMutex);
    while (!mMeshBufferRequestQueue.empty())
    {
      std::unique_ptr<MeshBufferRequest>& request =
        mMeshBufferRequestQueue.front();

      if (request != nullptr)
      {
        std::unique_lock<std::mutex> requestLock(request->RequestMutex);

        // requestLock.lock();
        request->Destination.get() = mMeshBufferFactory->Create(request->Mesh);
        // requestLock.unlock();
      }

      mMeshBufferRequestQueue.pop();
    }
  }
}