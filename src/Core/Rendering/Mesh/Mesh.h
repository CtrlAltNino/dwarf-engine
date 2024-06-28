#pragma once

#include "Core/Base.h"
#include "Vertex.h"

namespace Dwarf
{
  class Mesh
  {
  private:
    std::vector<Vertex>       m_Vertices = std::vector<Vertex>();
    std::vector<unsigned int> m_Indices = std::vector<unsigned int>();
    unsigned int              m_MaterialIndex = 0;

  public:
    Mesh(std::vector<Vertex>       vertices,
         std::vector<unsigned int> indices,
         unsigned int              materialIndex);
    virtual ~Mesh() = default;

    static std::shared_ptr<Mesh>
    Create(std::vector<Vertex> const&       vertices,
           std::vector<unsigned int> const& indices,
           unsigned int                     materialIndex);
    static std::shared_ptr<Mesh>
    GenerateUnitSphere(int stacks, int slices);
    static void
    Init();

    virtual void
    SetupMesh() = 0;

    int
    GetMaterialIndex() const;
    std::vector<Vertex>
    GetVertices() const;
    std::vector<unsigned int>
    GetIndices() const;

    static std::shared_ptr<Mesh> s_GridMesh;
    static std::shared_ptr<Mesh> s_UnitSphere;
  };
}