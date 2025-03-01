#include "MeshBufferWorker.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "IMeshBufferWorker.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  MeshBufferWorker::MeshBufferWorker(
    std::shared_ptr<IDwarfLogger>       logger,
    std::shared_ptr<IMeshBufferFactory> meshBufferFactory)
    : m_Logger(logger)
    , m_MeshBufferFactory(meshBufferFactory)
  {
    m_Logger->LogDebug(Log("MeshBufferWorker created.", "MeshBufferWorker"));
  }

  void
  MeshBufferWorker::RequestMeshBuffer(
    std::unique_ptr<MeshBufferRequest>&& request)
  {
    m_Logger->LogDebug(Log("Adding mesh buffer request", "MeshBufferWorker"));
    {
      std::lock_guard<std::mutex> lock(m_UploadMutex);
      m_MeshBufferRequestQueue.push(std::move(request));
    }
  }

  void
  MeshBufferWorker::ProcessRequests()
  {
    std::lock_guard<std::mutex> lock(m_UploadMutex);
    while (!m_MeshBufferRequestQueue.empty())
    {
      std::unique_ptr<MeshBufferRequest>& request =
        m_MeshBufferRequestQueue.front();

      request->Destination = m_MeshBufferFactory->Create(request->Mesh);
      m_MeshBufferRequestQueue.pop();
    }
  }
}