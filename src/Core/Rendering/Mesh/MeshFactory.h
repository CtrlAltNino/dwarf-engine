#pragma once

#include "Core/Base.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
namespace Dwarf
{
  class MeshFactory : public IMeshFactory
  {
  private:
    GraphicsApi m_GraphicsApi;

  public:
    MeshFactory(GraphicsApi graphicsApi);
    std::shared_ptr<IMesh>
    CreateMesh(std::vector<Vertex>       vertices,
               std::vector<unsigned int> indices,
               unsigned int              materialIndex) override;
  };
}