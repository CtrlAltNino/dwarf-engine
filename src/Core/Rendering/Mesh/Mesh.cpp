#include "pch.h"

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
    , mLogger(std::move(logger))
  {
    mLogger->LogDebug(Log("OpenGLMesh created.", "OpenGLMesh"));
  }

  Mesh::~Mesh()
  {
    mLogger->LogDebug(Log("Mesh destroyed.", "OpenGLMesh"));
  }

  auto
  Mesh::GetMaterialIndex() const -> unsigned int
  {
    return mMaterialIndex;
  }

  auto
  Mesh::GetVertices() const -> const std::vector<Vertex>&
  {
    return mVertices;
  }

  auto
  Mesh::GetIndices() const -> const std::vector<unsigned int>&
  {
    return mIndices;
  }

  auto
  Mesh::Clone() const -> std::unique_ptr<IMesh>
  {
    return std::make_unique<Mesh>(mVertices, mIndices, mMaterialIndex, mLogger);
  }
}