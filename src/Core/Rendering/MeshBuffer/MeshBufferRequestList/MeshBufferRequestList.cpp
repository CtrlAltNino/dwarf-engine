#include "pch.hpp"

#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "IMeshBufferRequestList.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "MeshBufferRequestList.hpp"

namespace Dwarf
{
  MeshBufferRequestList::MeshBufferRequestList(
    std::shared_ptr<IDwarfLogger>       logger,
    std::shared_ptr<IMeshBufferFactory> meshBufferFactory)
    : mLogger(std::move(logger))
    , mMeshBufferFactory(std::move(meshBufferFactory))
  {
    mLogger->LogDebug(
      Log("MeshBufferRequestList created.", "MeshBufferRequestList"));
  }

  MeshBufferRequestList::~MeshBufferRequestList()
  {
    mLogger->LogDebug(
      Log("MeshBufferRequestList destroyed.", "MeshBufferRequestList"));
  }

  void
  MeshBufferRequestList::RequestMeshBuffer(
    std::unique_ptr<MeshBufferRequest>&& request)
  {
    mLogger->LogDebug(
      Log("Adding mesh buffer request", "MeshBufferRequestList"));
    std::lock_guard<std::mutex> lock(mMeshBufferRequestMutex);
    mMeshBufferRequestQueue.push(std::move(request));
  }

  void
  MeshBufferRequestList::ProcessRequests()
  {
    std::lock_guard<std::mutex> lock(mMeshBufferRequestMutex);
    while (!mMeshBufferRequestQueue.empty())
    {
      std::unique_ptr<MeshBufferRequest>& request =
        mMeshBufferRequestQueue.front();

      if (request != nullptr)
      {
        std::unique_lock<std::mutex> requestLock(request->RequestMutex);
        request->OnFinish(mMeshBufferFactory->Create(request->Mesh));
        //*request->Destination = mMeshBufferFactory->Create(request->Mesh);
      }

      mMeshBufferRequestQueue.pop();
    }
  }

  auto
  MeshBufferRequestList::GetMutex() -> std::mutex&
  {
    return mMeshBufferRequestMutex;
  }

  void
  MeshBufferRequestList::ClearRequests()
  {
    std::lock_guard<std::mutex> lock(mMeshBufferRequestMutex);
    mMeshBufferRequestQueue = {};
  }
}