#include "DrawCall.h"
#include "IDrawCall.h"

namespace Dwarf
{
  DrawCall::DrawCall(IMesh& mesh, IMaterial& material, glm::mat4 modelMatrix)
    : m_Mesh(mesh)
    , m_Material(material)
    , m_ModelMatrix(modelMatrix)
  {
  }

  DrawCall::DrawCall(IDrawCall& other)
    : m_Mesh(other.GetMesh())
    , m_Material(other.GetMaterial())
    , m_ModelMatrix(other.GetModelMatrix())
  {
  }

  IMesh&
  DrawCall::GetMesh()
  {
    return m_Mesh;
  }

  IMaterial&
  DrawCall::GetMaterial()
  {
    return m_Material;
  }

  glm::mat4&
  DrawCall::GetModelMatrix()
  {
    return m_ModelMatrix;
  }
}