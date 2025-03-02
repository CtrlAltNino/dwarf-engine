#include "Mesh.h"

namespace Dwarf
{
  Mesh::Mesh(const std::vector<Vertex>&       vertices,
             const std::vector<unsigned int>& indices,
             unsigned int                     materialIndex,
             std::shared_ptr<IDwarfLogger>    logger)
    : m_Vertices(vertices)
    , m_Indices(indices)
    , m_MaterialIndex(materialIndex)
    , m_Logger(logger)
  {
    m_Logger->LogDebug(Log("OpenGLMesh created.", "OpenGLMesh"));
  }

  Mesh::~Mesh()
  {
    m_Logger->LogDebug(Log("Mesh destroyed.", "OpenGLMesh"));
  }

  int
  Mesh::GetMaterialIndex() const
  {
    return m_MaterialIndex;
  }

  std::vector<Vertex>
  Mesh::GetVertices() const
  {
    return m_Vertices;
  }

  std::vector<unsigned int>
  Mesh::GetIndices() const
  {
    return m_Indices;
  }

  std::unique_ptr<IMesh>
  Mesh::Clone() const
  {
    return std::make_unique<Mesh>(
      m_Vertices, m_Indices, m_MaterialIndex, m_Logger);
  }
}