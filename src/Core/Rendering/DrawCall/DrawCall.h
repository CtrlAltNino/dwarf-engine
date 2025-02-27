#pragma once

#include "IDrawCall.h"

namespace Dwarf
{
  class DrawCall : public IDrawCall
  {
  private:
    IMesh&     m_Mesh;
    IMaterial& m_Material;
    glm::mat4  m_ModelMatrix;

  public:
    DrawCall(IMesh& mesh, IMaterial& material, glm::mat4 modelMatrix);
    DrawCall(IDrawCall& other);

    ~DrawCall() override = default;

    IMesh&
    GetMesh() override;

    IMaterial&
    GetMaterial() override;

    glm::mat4&
    GetModelMatrix() override;
  };
}