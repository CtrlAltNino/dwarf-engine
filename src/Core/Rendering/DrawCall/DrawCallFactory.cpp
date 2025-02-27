#include "DrawCallFactory.h"
#include "DrawCall.h"

namespace Dwarf
{
  std::unique_ptr<IDrawCall>
  DrawCallFactory::Create(IMesh&     mesh,
                          IMaterial& material,
                          glm::mat4  modelMatrix)
  {
    return std::make_unique<DrawCall>(mesh, material, modelMatrix);
  }
}