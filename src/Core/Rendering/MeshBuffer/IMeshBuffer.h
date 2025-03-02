#pragma once

#include "Core/Rendering/Mesh/Vertex.h"

namespace Dwarf
{
  class IMeshBuffer
  {
  public:
    IMeshBuffer() = default;
    virtual ~IMeshBuffer() = default;

    virtual uint32_t
    GetVertexCount() = 0;

    virtual uint32_t
    GetIndexCount() = 0;

    IMeshBuffer(const IMeshBuffer&) = delete; // Delete copy constructor
    IMeshBuffer&
    operator=(const IMeshBuffer&) = delete; // Delete copy assignment operator
  };
}