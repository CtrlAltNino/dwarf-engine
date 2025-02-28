#include "DrawCallFactory.h"
#include "DrawCall.h"

namespace Dwarf
{
  std::unique_ptr<IDrawCall>
  DrawCallFactory::Create(std::unique_ptr<IMeshBuffer>&& mesh,
                          IMaterial&                     material,
                          TransformComponent&            transform)
  {
    return std::make_unique<DrawCall>(std::move(mesh), material, transform);
  }
}