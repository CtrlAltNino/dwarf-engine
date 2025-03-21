#include "MeshFactory.h"
#include "Logging/IDwarfLogger.h"
#include "Mesh.h"

namespace Dwarf
{
  MeshFactory::MeshFactory(std::shared_ptr<IDwarfLogger> logger)
    : m_Logger(logger)
  {
  }
  std::unique_ptr<IMesh>
  MeshFactory::Create(const std::vector<Vertex>&      vertices,
                      const std::vector<unsigned int> indices,
                      unsigned int                    materialIndex)
  {
    return std::make_unique<Mesh>(vertices, indices, materialIndex, m_Logger);
  }

  std::unique_ptr<IMesh>
  MeshFactory::CreateUnitSphere(int stacks, int slices)
  {
    auto vertices = std::vector<Vertex>();
    auto indices = std::vector<unsigned int>();
    for (int i = 0; i <= stacks; ++i)
    {
      float phi =
        glm::pi<float>() * static_cast<float>(i) / static_cast<float>(stacks);
      for (int j = 0; j <= slices; ++j)
      {
        float theta = 2.0f * glm::pi<float>() * static_cast<float>(j) /
                      static_cast<float>(slices);

        float x = std::sin(phi) * std::cos(theta);
        float y = std::cos(phi);
        float z = std::sin(phi) * std::sin(theta);

        // Compute the tangent (derivative of position with respect to theta)
        float     tx = -std::sin(phi) * std::sin(theta);
        float     ty = 0.0f;
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
        int index0 = i * (slices + 1) + j;
        int index1 = index0 + 1;
        int index2 = (i + 1) * (slices + 1) + j;
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

  std::unique_ptr<IMesh>
  MeshFactory::CreateUnitCube()
  {
    std::vector<Vertex> vertices = {
      // Positions          // Normals             // UVs
      // Front face
      Vertex(glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3(0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, 0.5f),
             glm::vec3(0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, 0.5f),
             glm::vec3(0.0f, 0.0f, 1.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, 0.5f),
             glm::vec3(0.0f, 0.0f, 1.0f),
             glm::vec2(0.0f, 1.0f)),

      // Back face
      Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(0.0f, 0.0f, -1.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, -0.5f),
             glm::vec3(0.0f, 0.0f, -1.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, -0.5f),
             glm::vec3(0.0f, 0.0f, -1.0f),
             glm::vec2(0.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, -0.5f),
             glm::vec3(0.0f, 0.0f, -1.0f),
             glm::vec2(1.0f, 1.0f)),

      // Left face
      Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(-1.0f, 0.0f, 0.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3(-1.0f, 0.0f, 0.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, 0.5f),
             glm::vec3(-1.0f, 0.0f, 0.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, -0.5f),
             glm::vec3(-1.0f, 0.0f, 0.0f),
             glm::vec2(0.0f, 1.0f)),

      // Right face
      Vertex(glm::vec3(0.5f, -0.5f, -0.5f),
             glm::vec3(1.0f, 0.0f, 0.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, 0.5f),
             glm::vec3(1.0f, 0.0f, 0.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, 0.5f),
             glm::vec3(1.0f, 0.0f, 0.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, -0.5f),
             glm::vec3(1.0f, 0.0f, 0.0f),
             glm::vec2(0.0f, 1.0f)),

      // Top face
      Vertex(glm::vec3(-0.5f, 0.5f, -0.5f),
             glm::vec3(0.0f, 1.0f, 0.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, -0.5f),
             glm::vec3(0.0f, 1.0f, 0.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, 0.5f, 0.5f),
             glm::vec3(0.0f, 1.0f, 0.0f),
             glm::vec2(1.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, 0.5f, 0.5f),
             glm::vec3(0.0f, 1.0f, 0.0f),
             glm::vec2(0.0f, 1.0f)),

      // Bottom face
      Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(0.0f, -1.0f, 0.0f),
             glm::vec2(1.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, -0.5f),
             glm::vec3(0.0f, -1.0f, 0.0f),
             glm::vec2(0.0f, 0.0f)),
      Vertex(glm::vec3(0.5f, -0.5f, 0.5f),
             glm::vec3(0.0f, -1.0f, 0.0f),
             glm::vec2(0.0f, 1.0f)),
      Vertex(glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3(0.0f, -1.0f, 0.0f),
             glm::vec2(1.0f, 1.0f)),
    };

    std::vector<unsigned int> indices = { // Front face
                                          0,
                                          1,
                                          2,
                                          2,
                                          3,
                                          0,
                                          // Back face
                                          4,
                                          5,
                                          6,
                                          6,
                                          7,
                                          4,
                                          // Left face
                                          8,
                                          9,
                                          10,
                                          10,
                                          11,
                                          8,
                                          // Right face
                                          12,
                                          13,
                                          14,
                                          14,
                                          15,
                                          12,
                                          // Top face
                                          16,
                                          17,
                                          18,
                                          18,
                                          19,
                                          16,
                                          // Bottom face
                                          20,
                                          21,
                                          22,
                                          22,
                                          23,
                                          20
    };
    return Create(vertices, indices, 0);
  }

  std::unique_ptr<IMesh>
  MeshFactory::CreateUnitQuad()
  {
    return Create(
      {
        { glm::vec3(-1.0f, 1.0f, 0.0f),
          glm::vec3(0.0f, 0.0f, 1.0f),
          glm::vec2(0.0f, 1.0f) }, // 0
        { glm::vec3(-1.0f, -1.0f, 0.0f),
          glm::vec3(0.0f, 0.0f, 1.0f),
          glm::vec2(0.0f, 0.0f) }, // 1
        { glm::vec3(1.0f, -1.0f, 0.0f),
          glm::vec3(0.0f, 0.0f, 1.0f),
          glm::vec2(1.0f, 0.0f) }, // 2
        { glm::vec3(1.0f, 1.0f, 0.0f),
          glm::vec3(0.0f, 0.0f, 1.0f),
          glm::vec2(1.0f, 1.0f) } // 3
      },
      {
        0,
        1,
        2, // First Triangle
        0,
        2,
        3 // Second Triangle
      },
      0);
  }

  std::unique_ptr<IMesh>
  MeshFactory::CreateFullscreenQuad()
  {
    return Create(
      { { { -1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
        { { 1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
        { { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        { { -1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } } },
      { 0, 1, 2, 2, 3, 0 },
      0);
  }

  std::unique_ptr<IMesh>
  MeshFactory::MergeMeshes(const std::vector<std::unique_ptr<IMesh>>& meshes)
  {
    std::vector<Vertex>       mergedVertices;
    std::vector<unsigned int> mergedIndices;
    uint32_t indexOffset = 0; // Tracks index shifting due to merged vertices

    for (auto& mesh : meshes)
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

  std::unique_ptr<IMesh>
  MeshFactory::MergeMeshes(
    const std::vector<std::reference_wrapper<IMesh>>& meshes)
  {
    std::vector<Vertex>       mergedVertices;
    std::vector<unsigned int> mergedIndices;
    uint32_t indexOffset = 0; // Tracks index shifting due to merged vertices

    for (auto& mesh : meshes)
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