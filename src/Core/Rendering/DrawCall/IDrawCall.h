#pragma once

#include "Core/Rendering/Material/IMaterial.h"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.h"
#include "Core/Scene/Components/SceneComponents.h"
#include <glm/fwd.hpp>
namespace Dwarf
{
  class IDrawCall
  {
  public:
    virtual ~IDrawCall() = default;

    virtual std::unique_ptr<IMeshBuffer>&
    GetMeshBuffer() = 0;

    virtual IMaterial&
    GetMaterial() = 0;

    virtual TransformComponent&
    GetTransform() = 0;
  };
}