#pragma once

#include "Core/Rendering/Mesh/IMesh.h"
namespace Dwarf
{
  class IMeshFactory
  {
  public:
    virtual ~IMeshFactory() = default;

    virtual std::shared_ptr<IMesh>
    CreateMesh(std::vector<Vertex>       vertices,
               std::vector<unsigned int> indices,
               unsigned int              materialIndex) = 0;
  };
}