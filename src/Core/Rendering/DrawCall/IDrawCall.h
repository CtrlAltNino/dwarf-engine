#pragma once

#include "Core/Rendering/Material/IMaterial.h"
#include "Core/Rendering/Mesh/IMesh.h"
#include <glm/fwd.hpp>
namespace Dwarf
{
  class IDrawCall
  {
  public:
    virtual ~IDrawCall() = default;

    virtual IMesh&
    GetMesh() = 0;

    virtual IMaterial&
    GetMaterial() = 0;

    virtual glm::mat4&
    GetModelMatrix() = 0;
  };
}