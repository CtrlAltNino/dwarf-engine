#pragma once

#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "IMeshBufferRequestList.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <mutex>
#include <queue>

namespace Dwarf
{
  class MeshBufferRequestList : public IMeshBufferRequestList
  {
  private:
    std::shared_ptr<IDwarfLogger>                  mLogger;
    std::shared_ptr<IMeshBufferFactory>            mMeshBufferFactory;
    std::queue<std::unique_ptr<MeshBufferRequest>> mMeshBufferRequestQueue;
    std::mutex                                     mMeshBufferRequestMutex;

  public:
    MeshBufferRequestList(
      std::shared_ptr<IDwarfLogger>       logger,
      std::shared_ptr<IMeshBufferFactory> meshBufferFactory);
    ~MeshBufferRequestList() override;

    /**
     * @brief Enqueues a request to create a mesh buffer
     *
     * @param request The request data
     */
    void
    RequestMeshBuffer(std::unique_ptr<MeshBufferRequest>&& request) override;

    /**
     * @brief Processes all queued requests (Should be called on the thread
     * where uploading is possible)
     *
     */
    void
    ProcessRequests() override;

    auto
    GetMutex() -> std::mutex& override;

    void
    ClearRequests() override;
  };
}