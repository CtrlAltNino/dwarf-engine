#include "pch.hpp"

#include "Mesh.hpp"

namespace Dwarf
{
  Mesh::Mesh(const std::vector<Vertex>&    vertices,
             const std::vector<uint32_t>&  indices,
             uint32_t                      materialIndex,
             std::shared_ptr<IDwarfLogger> logger)
    : mVertices(vertices)
    , mIndices(indices)
    , mMaterialIndex(materialIndex)
    , mLogger(std::move(logger))
  {
    mLogger->LogDebug(Log("Mesh created.", "Mesh"));
  }

  Mesh::~Mesh()
  {
    mLogger->LogDebug(Log("Mesh destroyed.", "Mesh"));
  }

  auto
  Mesh::GetMaterialIndex() const -> uint32_t
  {
    return mMaterialIndex;
  }

  auto
  Mesh::GetVertices() const -> const std::vector<Vertex>&
  {
    return mVertices;
  }

  auto
  Mesh::GetIndices() const -> const std::vector<uint32_t>&
  {
    return mIndices;
  }

  auto
  Mesh::Clone() const -> std::unique_ptr<IMesh>
  {
    return std::make_unique<Mesh>(mVertices, mIndices, mMaterialIndex, mLogger);
  }
}