#pragma once

#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "IMeshBufferWorker.h"
#include "Core/Rendering/Mesh/IMesh.h"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.h"
#include "Logging/IDwarfLogger.h"
#include <mutex>
#include <queue>
namespace Dwarf
{
  class MeshBufferWorker : public IMeshBufferWorker
  {
  private:
    std::shared_ptr<IDwarfLogger>                  m_Logger;
    std::shared_ptr<IMeshBufferFactory>            m_MeshBufferFactory;
    std::queue<std::unique_ptr<MeshBufferRequest>> m_MeshBufferRequestQueue;
    std::mutex                                     m_UploadMutex;

  public:
    MeshBufferWorker(std::shared_ptr<IDwarfLogger>       logger,
                     std::shared_ptr<IMeshBufferFactory> meshBufferFactory);
    ~MeshBufferWorker() override = default;

    void
    RequestMeshBuffer(std::unique_ptr<MeshBufferRequest>&& request) override;

    void
    ProcessRequests() override;
  };
}