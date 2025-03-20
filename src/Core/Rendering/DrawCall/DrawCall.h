#pragma once

#include "Core/Scene/Components/SceneComponents.h"
#include "IDrawCall.h"

namespace Dwarf
{
  class DrawCall : public IDrawCall
  {
  private:
    std::unique_ptr<IMeshBuffer> mMeshBuffer;
    IMaterial&                   mMaterial;
    TransformComponent&          mTransform;

  public:
    DrawCall(std::unique_ptr<IMeshBuffer>&& meshBuffer,
             IMaterial&                     material,
             TransformComponent&            transform);

    ~DrawCall() override = default;

    std::unique_ptr<IMeshBuffer>&
    GetMeshBuffer() override;

    IMaterial&
    GetMaterial() override;

    TransformComponent&
    GetTransform() override;
  };
}