#include "DrawCall.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "IDrawCall.h"

namespace Dwarf
{
  DrawCall::DrawCall(std::unique_ptr<IMeshBuffer>&& meshBuffer,
                     IMaterial&                     material,
                     TransformComponent&            transform)
    : m_MeshBuffer(std::move(meshBuffer))
    , m_Material(material)
    , m_Transform(transform)
  {
  }

  IMeshBuffer&
  DrawCall::GetMeshBuffer()
  {
    return *m_MeshBuffer;
  }

  IMaterial&
  DrawCall::GetMaterial()
  {
    return m_Material;
  }

  TransformComponent&
  DrawCall::GetTransform()
  {
    return m_Transform;
  }
}