#pragma once

#include "IMesh.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class Mesh : public IMesh
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    std::vector<Vertex>           m_Vertices = std::vector<Vertex>();
    std::vector<unsigned int>     m_Indices = std::vector<unsigned int>();
    unsigned int                  m_MaterialIndex = 0;

  public:
    Mesh(const std::vector<Vertex>&       vertices,
         const std::vector<unsigned int>& indices,
         unsigned int                     materialIndex,
         std::shared_ptr<IDwarfLogger>    logger);
    ~Mesh() override;

    int
    GetMaterialIndex() const override;
    std::vector<Vertex>
    GetVertices() const override;
    std::vector<unsigned int>
    GetIndices() const override;

    std::unique_ptr<IMesh>
    Clone() const override;
  };
}