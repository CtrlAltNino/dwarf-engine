#pragma once

#include "Core/Rendering/Mesh/IMesh.h"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.h"
namespace Dwarf
{
  struct MeshBufferRequest
  {
    std::unique_ptr<IMeshBuffer>& Destination;
    std::unique_ptr<IMesh>        Mesh;

    MeshBufferRequest(std::unique_ptr<IMeshBuffer>& destination,
                      std::unique_ptr<IMesh>&&      mesh)
      : Destination(destination)
      , Mesh(std::move(mesh))
    {
    }
  };

  class IMeshBufferWorker
  {
  public:
    virtual ~IMeshBufferWorker() = default;
    virtual void
    RequestMeshBuffer(std::unique_ptr<MeshBufferRequest>&& request) = 0;

    virtual void
    ProcessRequests() = 0;
  };
}