#pragma once

#include "Core/Rendering/Mesh/Vertex.h"

namespace Dwarf
{
  class IMeshBuffer
  {
  public:
    virtual ~IMeshBuffer() = default;

    virtual uint32_t
    GetVertexCount() = 0;

    virtual uint32_t
    GetIndexCount() = 0;
  };
}