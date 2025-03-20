#include "Mesh.h"

namespace Dwarf
{
  Mesh::Mesh(const std::vector<Vertex>&       vertices,
             const std::vector<unsigned int>& indices,
             unsigned int                     materialIndex,
             std::shared_ptr<IDwarfLogger>    logger)
    : mVertices(vertices)
    , mIndices(indices)
    , mMaterialIndex(materialIndex)
    , mLogger(logger)
  {
    mLogger->LogDebug(Log("OpenGLMesh created.", "OpenGLMesh"));
  }

  Mesh::~Mesh()
  {
    mLogger->LogDebug(Log("Mesh destroyed.", "OpenGLMesh"));
  }

  int
  Mesh::GetMaterialIndex() const
  {
    return mMaterialIndex;
  }

  std::vector<Vertex>
  Mesh::GetVertices() const
  {
    return mVertices;
  }

  std::vector<unsigned int>
  Mesh::GetIndices() const
  {
    return mIndices;
  }

  std::unique_ptr<IMesh>
  Mesh::Clone() const
  {
    return std::make_unique<Mesh>(mVertices, mIndices, mMaterialIndex, mLogger);
  }
}