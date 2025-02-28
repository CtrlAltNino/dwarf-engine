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
    Create(std::unique_ptr<IMeshBuffer>&& mesh,
           IMaterial&                     material,
           TransformComponent&            transform) override;
  };
}