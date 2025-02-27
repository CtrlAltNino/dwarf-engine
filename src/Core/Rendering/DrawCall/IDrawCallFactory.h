#pragma once

#include "IDrawCall.h"
namespace Dwarf
{
  class IDrawCallFactory
  {
  public:
    virtual ~IDrawCallFactory() = default;

    virtual std::unique_ptr<IDrawCall>
    Create(IMesh& mesh, IMaterial& material, glm::mat4 modelMatrix) = 0;
  };
}