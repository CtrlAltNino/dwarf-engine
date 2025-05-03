#include "pch.hpp"

#include "DrawCall.hpp"

namespace Dwarf
{
  DrawCall::DrawCall(std::unique_ptr<IMeshBuffer>&& meshBuffer,
                     MaterialAsset&                 material,
                     TransformComponentHandle       transform)
    : mMeshBuffer(std::move(meshBuffer))
    , mMaterial(material)
    , mTransform(transform)
  {
  }

  auto
  DrawCall::GetMeshBuffer() -> const IMeshBuffer*
  {
    return mMeshBuffer.get();
  }

  void
  DrawCall::SetMeshBuffer(std::unique_ptr<IMeshBuffer>&& meshBuffer)
  {
    mMeshBuffer = std::move(meshBuffer);
  }

  auto
  DrawCall::GetMaterialAsset() -> MaterialAsset&
  {
    return mMaterial;
  }

  auto
  DrawCall::GetTransform() -> TransformComponentHandle
  {
    return mTransform;
  }
}