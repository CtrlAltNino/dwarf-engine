#pragma once

#include "Core/Scene/Components/SceneComponents.h"
#include "IDrawCall.h"

namespace Dwarf
{
  class DrawCall : public IDrawCall
  {
  private:
    std::unique_ptr<IMeshBuffer> m_MeshBuffer;
    IMaterial&                   m_Material;
    TransformComponent&          m_Transform;

  public:
    DrawCall(std::unique_ptr<IMeshBuffer>&& meshBuffer,
             IMaterial&                     material,
             TransformComponent&            transform);

    ~DrawCall() override = default;

    IMeshBuffer&
    GetMeshBuffer() override;

    IMaterial&
    GetMaterial() override;

    TransformComponent&
    GetTransform() override;
  };
}