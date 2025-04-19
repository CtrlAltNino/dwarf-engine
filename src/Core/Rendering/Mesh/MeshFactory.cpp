#include "pch.hpp"

#include "Logging/IDwarfLogger.hpp"
#include "Mesh.hpp"
#include "MeshFactory.hpp"

namespace Dwarf
{
  MeshFactory::MeshFactory(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
  {
  }
  auto
  MeshFactory::Create(const std::vector<Vertex>&   vertices,
                      const std::vector<uint32_t>& indices,
                      uint32_t materialIndex) const -> std::unique_ptr<IMesh>
  {
    return std::make_unique<Mesh>(vertices, indices, materialIndex, mLogger);
  }

  auto
  MeshFactory::CreateUnitSphere(int stacks,
                                int slices) const -> std::unique_ptr<IMesh>
  {
    auto vertices = std::vector<Vertex>();
    auto indices = std::vector<uint32_t>();
    for (int i = 0; i <= stacks; ++i)
    {
      float phi =
        glm::pi<float>() * static_cast<float>(i) / static_cast<float>(stacks);
      for (int j = 0; j <= slices; ++j)
      {
        float theta = 2.0F * glm::pi<float>() * static_cast<float>(j) /
                      static_cast<float>(slices);

        float x = std::sin(phi) * std::cos(theta);
        float y = std::cos(phi);
        float z = std::sin(phi) * std::sin(theta);

        // Compute the tangent (derivative of position with respect to theta)
        float     tx = -std::sin(phi) * std::sin(theta);
        float     ty = 0.0F;
        float     tz = std::sin(phi) * std::cos(theta);
        glm::vec3 tangent = glm::normalize(glm::vec3(tx, ty, tz));

        // Compute the bitangent (derivative of position with respect to phi)
        float     bx = std::cos(phi) * std::cos(theta);
        float     by = -std::sin(phi);
        float     bz = std::cos(phi) * std::sin(theta);
        glm::vec3 bitangent = glm::normalize(glm::vec3(bx, by, bz));

        Vertex vert({ x, y, z },
                    glm::normalize(glm::vec3(x, y, z)),
                    tangent,
                    bitangent,
                    { static_cast<float>(j) / static_cast<float>(slices),
                      static_cast<float>(i) / static_cast<float>(stacks) });

        vertices.push_back(vert);
      }
    }

    for (int i = 0; i < stacks; ++i)
    {
      for (int j = 0; j < slices; ++j)
      {
        int index0 = (i * (slices + 1)) + j;
        int index1 = index0 + 1;
        int index2 = ((i + 1) * (slices + 1)) + j;
        int index3 = index2 + 1;

        indices.push_back(index0);
        indices.push_back(index1);
        indices.push_back(index2);

        indices.push_back(index2);
        indices.push_back(index1);
        indices.push_back(index3);
      }
    }

    return Create(vertices, indices, 0);
  }

  auto
  MeshFactory::CreateUnitCube() const -> std::unique_ptr<IMesh>
  {
    float               half = 0.7F;
    std::vector<Vertex> vertices = {
      // Front face
      { { -half, -half, half },
        { 0, 0, 1 },
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 1 } },
      { { half, -half, half },
        { 0, 0, 1 },
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 1, 1 } },
      { { half, half, half }, { 0, 0, 1 }, { 1, 0, 0 }, { 0, 1, 0 }, { 1, 0 } },
      { { -half, half, half },
        { 0, 0, 1 },
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0 } },

      // Back face
      { { half, -half, -half },
        { 0, 0, -1 },
        { -1, 0, 0 },
        { 0, 1, 0 },
        { 0, 1 } },
      { { -half, -half, -half },
        { 0, 0, -1 },
        { -1, 0, 0 },
        { 0, 1, 0 },
        { 1, 1 } },
      { { -half, half, -half },
        { 0, 0, -1 },
        { -1, 0, 0 },
        { 0, 1, 0 },
        { 1, 0 } },
      { { half, half, -half },
        { 0, 0, -1 },
        { -1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0 } },

      // Left face
      { { -half, -half, -half },
        { -1, 0, 0 },
        { 0, 0, 1 },
        { 0, 1, 0 },
        { 0, 1 } },
      { { -half, -half, half },
        { -1, 0, 0 },
        { 0, 0, 1 },
        { 0, 1, 0 },
        { 1, 1 } },
      { { -half, half, half },
        { -1, 0, 0 },
        { 0, 0, 1 },
        { 0, 1, 0 },
        { 1, 0 } },
      { { -half, half, -half },
        { -1, 0, 0 },
        { 0, 0, 1 },
        { 0, 1, 0 },
        { 0, 0 } },

      // Right face
      { { half, -half, half },
        { 1, 0, 0 },
        { 0, 0, -1 },
        { 0, 1, 0 },
        { 0, 1 } },
      { { half, -half, -half },
        { 1, 0, 0 },
        { 0, 0, -1 },
        { 0, 1, 0 },
        { 1, 1 } },
      { { half, half, -half },
        { 1, 0, 0 },
        { 0, 0, -1 },
        { 0, 1, 0 },
        { 1, 0 } },
      { { half, half, half },
        { 1, 0, 0 },
        { 0, 0, -1 },
        { 0, 1, 0 },
        { 0, 0 } },

      // Top face
      { { -half, half, half },
        { 0, 1, 0 },
        { 1, 0, 0 },
        { 0, 0, -1 },
        { 0, 1 } },
      { { half, half, half },
        { 0, 1, 0 },
        { 1, 0, 0 },
        { 0, 0, -1 },
        { 1, 1 } },
      { { half, half, -half },
        { 0, 1, 0 },
        { 1, 0, 0 },
        { 0, 0, -1 },
        { 1, 0 } },
      { { -half, half, -half },
        { 0, 1, 0 },
        { 1, 0, 0 },
        { 0, 0, -1 },
        { 0, 0 } },

      // Bottom face
      { { -half, -half, -half },
        { 0, -1, 0 },
        { 1, 0, 0 },
        { 0, 0, 1 },
        { 0, 1 } },
      { { half, -half, -half },
        { 0, -1, 0 },
        { 1, 0, 0 },
        { 0, 0, 1 },
        { 1, 1 } },
      { { half, -half, half },
        { 0, -1, 0 },
        { 1, 0, 0 },
        { 0, 0, 1 },
        { 1, 0 } },
      { { -half, -half, half },
        { 0, -1, 0 },
        { 1, 0, 0 },
        { 0, 0, 1 },
        { 0, 0 } },
    };

    std::vector<uint32_t> indices = {
      0,  1,  2,  2,  3,  0,  // Front
      4,  5,  6,  6,  7,  4,  // Back
      8,  9,  10, 10, 11, 8,  // Left
      12, 13, 14, 14, 15, 12, // Right
      16, 17, 18, 18, 19, 16, // Top
      20, 21, 22, 22, 23, 20  // Bottom
    };

    return Create(vertices, indices, 0);
  }

  auto
  MeshFactory::CreatePlane() const -> std::unique_ptr<IMesh>
  {
    float                 width = 1.0F;
    float                 depth = 1.0F;
    float                 halfW = width * 0.5f;
    float                 halfD = depth * 0.5f;
    auto                  vertices = std::vector<Vertex>();
    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

    glm::vec3 normal = glm::vec3(0, 1, 0);
    glm::vec3 tangent = glm::vec3(1, 0, 0);
    glm::vec3 bitangent = glm::vec3(0, 0, -1);

    std::vector<glm::vec3> positions = { { -halfW, 0, -halfD },
                                         { halfW, 0, -halfD },
                                         { halfW, 0, halfD },
                                         { -halfW, 0, halfD } };

    std::vector<glm::vec2> uvCoords = {
      { 0, 0 }, { 1, 0 }, { 1, 1 }, { 0, 1 }
    };

    for (size_t i = 0; i < 4; ++i)
    {
      vertices.emplace_back(
        positions[i], normal, tangent, bitangent, uvCoords[i]);
    }

    return Create(vertices, indices, 0);
  }

  auto
  MeshFactory::CreatePreviewQuad() const -> std::unique_ptr<IMesh>
  {
    float                 halfW = 0.9F;
    float                 halfD = 0.9F;
    auto                  vertices = std::vector<Vertex>();
    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

    glm::vec3 normal = glm::vec3(0, 0, 1);
    glm::vec3 tangent = glm::vec3(1, 0, 0);
    glm::vec3 bitangent = glm::vec3(0, 1, 0);

    std::vector<glm::vec3> positions = { { -halfW, -halfD, 0 },
                                         { halfW, -halfD, 0 },
                                         { halfW, halfD, 0 },
                                         { -halfW, halfD, 0 } };

    std::vector<glm::vec2> uvCoords = {
      { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 }
    };

    for (size_t i = 0; i < 4; ++i)
    {
      vertices.emplace_back(
        positions[i], normal, tangent, bitangent, uvCoords[i]);
    }

    return Create(vertices, indices, 0);
  }

  auto
  MeshFactory::CreateFullscreenQuad() const -> std::unique_ptr<IMesh>
  {
    return Create(
      { { { -1.0F, -1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F }, { 0.0F, 0.0F } },
        { { 1.0F, -1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F }, { 1.0F, 0.0F } },
        { { 1.0F, 1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F }, { 1.0F, 1.0F } },
        { { -1.0F, 1.0F, 0.0F }, { 0.0F, 0.0F, 1.0F }, { 0.0F, 1.0F } } },
      { 0, 1, 2, 2, 3, 0 },
      0);
  }

  auto
  MeshFactory::MergeMeshes(const std::vector<std::unique_ptr<IMesh>>& meshes)
    const -> std::unique_ptr<IMesh>
  {
    std::vector<Vertex>   mergedVertices;
    std::vector<uint32_t> mergedIndices;
    uint32_t indexOffset = 0; // Tracks index shifting due to merged vertices

    for (const auto& mesh : meshes)
    {
      for (auto vert : mesh->GetVertices())
      {
        mergedVertices.push_back(vert);
      }

      for (auto index : mesh->GetIndices())
      {
        mergedIndices.push_back(index + indexOffset);
      }

      indexOffset += mesh->GetVertices().size();
    }

    std::unique_ptr<IMesh> mergedMesh =
      Create(mergedVertices, mergedIndices, 0);

    return std::move(mergedMesh);
  }

  auto
  MeshFactory::MergeMeshes(const std::vector<std::reference_wrapper<IMesh>>&
                             meshes) const -> std::unique_ptr<IMesh>
  {
    std::vector<Vertex>   mergedVertices;
    std::vector<uint32_t> mergedIndices;
    uint32_t indexOffset = 0; // Tracks index shifting due to merged vertices

    for (const auto& mesh : meshes)
    {
      for (auto vert : mesh.get().GetVertices())
      {
        mergedVertices.push_back(vert);
      }

      for (auto index : mesh.get().GetIndices())
      {
        mergedIndices.push_back(index + indexOffset);
      }

      indexOffset += mesh.get().GetVertices().size();
    }

    std::unique_ptr<IMesh> mergedMesh =
      Create(mergedVertices, mergedIndices, 0);

    return std::move(mergedMesh);
  }
}