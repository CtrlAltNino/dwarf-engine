#pragma once

#include "Core/Rendering/Mesh/IMesh.h"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.h"
namespace Dwarf
{
  class IMeshBufferFactory
  {
  public:
    virtual ~IMeshBufferFactory() = default;

    virtual std::unique_ptr<IMeshBuffer>
    Create(std::unique_ptr<IMesh>& mesh) = 0;
  };
}