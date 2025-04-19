#pragma once

#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "IMeshBufferWorker.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <mutex>
#include <queue>

namespace Dwarf
{
  class MeshBufferWorker : public IMeshBufferWorker
  {
  private:
    std::shared_ptr<IDwarfLogger>                  mLogger;
    std::shared_ptr<IMeshBufferFactory>            mMeshBufferFactory;
    std::queue<std::unique_ptr<MeshBufferRequest>> mMeshBufferRequestQueue;
    std::mutex                                     mUploadMutex;

  public:
    MeshBufferWorker(std::shared_ptr<IDwarfLogger>       logger,
                     std::shared_ptr<IMeshBufferFactory> meshBufferFactory);
    ~MeshBufferWorker() override;

    /**
     * @brief Enqueues a request to create a mesh buffer
     *
     * @param request The request data
     */
    void
    RequestMeshBuffer(std::unique_ptr<MeshBufferRequest>&& request) override;

    /**
     * @brief Processes all queued requests
     *
     */
    void
    ProcessRequests() override;
  };
}