#include "DrawCall.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "IDrawCall.h"

namespace Dwarf
{
  DrawCall::DrawCall(std::unique_ptr<IMeshBuffer>&& meshBuffer,
                     IMaterial&                     material,
                     TransformComponent&            transform)
    : mMeshBuffer(std::move(meshBuffer))
    , mMaterial(material)
    , mTransform(transform)
  {
  }

  std::unique_ptr<IMeshBuffer>&
  DrawCall::GetMeshBuffer()
  {
    return mMeshBuffer;
  }

  IMaterial&
  DrawCall::GetMaterial()
  {
    return mMaterial;
  }

  TransformComponent&
  DrawCall::GetTransform()
  {
    return mTransform;
  }
}