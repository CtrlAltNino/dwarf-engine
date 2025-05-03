#pragma once

#include "Core/Rendering/Mesh/IMesh.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.hpp"

namespace Dwarf
{
  /**
   * @brief Struct that contains the mesh to upload and a pointer to put the
   * created mesh buffer
   *
   */
  struct MeshBufferRequest
  {
    std::function<void(std::unique_ptr<IMeshBuffer>&&)> OnFinish;
    std::shared_ptr<IMesh>                              Mesh;
    std::mutex                                          RequestMutex;

    MeshBufferRequest(
      std::function<void(std::unique_ptr<IMeshBuffer>&&)> onFinish,
      std::unique_ptr<IMesh>&&                            mesh)
      : OnFinish(std::move(onFinish))
      , Mesh(std::move(mesh))
    {
    }
  };

  /**
   * @brief Class to multi thread the creation of mesh buffers
   *
   */
  class IMeshBufferWorker
  {
  public:
    virtual ~IMeshBufferWorker() = default;

    /**
     * @brief Enqueues a request to create a mesh buffer
     *
     * @param request The request data
     */
    virtual void
    RequestMeshBuffer(std::unique_ptr<MeshBufferRequest>&& request) = 0;

    /**
     * @brief Processes all queued requests
     *
     */
    virtual void
    ProcessRequests() = 0;

    virtual void
    ClearRequests() = 0;
  };
}