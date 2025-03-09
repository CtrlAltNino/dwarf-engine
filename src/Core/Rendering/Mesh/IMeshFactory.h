#pragma once

#include "IMesh.h"

namespace Dwarf
{
  class IMeshFactory
  {
  public:
    virtual ~IMeshFactory() = default;

    virtual std::unique_ptr<IMesh>
    Create(const std::vector<Vertex>&      vertices,
           const std::vector<unsigned int> indices,
           unsigned int                    materialIndex) = 0;

    virtual std::unique_ptr<IMesh>
    CreateUnitSphere(int stacks, int slices) = 0;

    virtual std::unique_ptr<IMesh>
    CreateUnitCube() = 0;

    virtual std::unique_ptr<IMesh>
    CreateUnitQuad() = 0;

    virtual std::unique_ptr<IMesh>
    CreateFullscreenQuad() = 0;

    virtual std::unique_ptr<IMesh>
    MergeMeshes(const std::vector<std::unique_ptr<IMesh>>& meshes) = 0;

    virtual std::unique_ptr<IMesh>
    MergeMeshes(const std::vector<std::reference_wrapper<IMesh>>& meshes) = 0;
  };
}