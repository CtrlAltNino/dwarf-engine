#pragma once

#include "IDrawCall.h"
#include "IDrawCallFactory.h"
namespace Dwarf
{
  class DrawCallFactory : public IDrawCallFactory
  {
  public:
    ~DrawCallFactory() override = default;

    std::unique_ptr<IDrawCall>
    Create(IMesh& mesh, IMaterial& material, glm::mat4 modelMatrix) override;
  };
}