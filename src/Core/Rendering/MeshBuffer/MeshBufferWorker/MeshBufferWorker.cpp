#include "MeshBufferWorker.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "IMeshBufferWorker.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  MeshBufferWorker::MeshBufferWorker(
    std::shared_ptr<IDwarfLogger>       logger,
    std::shared_ptr<IMeshBufferFactory> meshBufferFactory)
    : mLogger(logger)
    , mMeshBufferFactory(meshBufferFactory)
  {
    mLogger->LogDebug(Log("MeshBufferWorker created.", "MeshBufferWorker"));
  }

  void
  MeshBufferWorker::RequestMeshBuffer(
    std::unique_ptr<MeshBufferRequest>&& request)
  {
    mLogger->LogDebug(Log("Adding mesh buffer request", "MeshBufferWorker"));
    {
      std::lock_guard<std::mutex> lock(mUploadMutex);
      mMeshBufferRequestQueue.push(std::move(request));
    }
  }

  void
  MeshBufferWorker::ProcessRequests()
  {
    std::lock_guard<std::mutex> lock(mUploadMutex);
    while (!mMeshBufferRequestQueue.empty())
    {
      std::unique_ptr<MeshBufferRequest>& request =
        mMeshBufferRequestQueue.front();

      request->Destination = mMeshBufferFactory->Create(request->Mesh);
      mMeshBufferRequestQueue.pop();
    }
  }
}