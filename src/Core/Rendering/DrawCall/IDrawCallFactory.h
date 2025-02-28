#pragma once

#include "Core/Scene/Components/SceneComponents.h"
#include "IDrawCall.h"
namespace Dwarf
{
  class IDrawCallFactory
  {
  public:
    virtual ~IDrawCallFactory() = default;

    virtual std::unique_ptr<IDrawCall>
    Create(std::unique_ptr<IMeshBuffer>&& mesh,
           IMaterial&                     material,
           TransformComponent&            transform) = 0;
  };
}