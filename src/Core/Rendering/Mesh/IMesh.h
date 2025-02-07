#pragma once

#include "Core/Rendering/Mesh/Vertex.h"

namespace Dwarf
{
  class IMesh
  {
  public:
    virtual ~IMesh() = default;

    virtual void
    SetupMesh() = 0;

    virtual int
    GetMaterialIndex() const = 0;

    virtual std::vector<Vertex>
    GetVertices() const = 0;

    virtual std::vector<unsigned int>
    GetIndices() const = 0;

    // clone function
    virtual std::unique_ptr<IMesh>
    Clone() const = 0;
  };
}