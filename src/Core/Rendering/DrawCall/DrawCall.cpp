#include "pch.h"

#include "Core/Scene/Components/SceneComponents.h"
#include "DrawCall.h"

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

  auto
  DrawCall::GetMeshBuffer() -> std::unique_ptr<IMeshBuffer>&
  {
    return mMeshBuffer;
  }

  auto
  DrawCall::GetMaterial() -> IMaterial&
  {
    return mMaterial;
  }

  auto
  DrawCall::GetTransform() -> TransformComponent&
  {
    return mTransform;
  }
}