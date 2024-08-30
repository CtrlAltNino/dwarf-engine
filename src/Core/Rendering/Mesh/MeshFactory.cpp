#include "MeshFactory.h"

#if _WIN32
#include "Platform/OpenGL/OpenGLMesh.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLMesh.h"
#elif __APPLE__
// #include "Platform/Metal/MetalShader.h"
#endif

namespace Dwarf
{
  MeshFactory::MeshFactory(GraphicsApi graphicsApi)
    : m_GraphicsApi(graphicsApi)
  {
  }

  std::shared_ptr<IMesh>
  MeshFactory::CreateMesh(std::vector<Vertex>       vertices,
                          std::vector<unsigned int> indices,
                          unsigned int              materialIndex)
  {
    // Creating a shader based on the graphics API.
    switch (m_GraphicsApi)
    {
#if _WIN32
      case GraphicsApi::D3D12:
        // return std::make_shared<D3D12Shader>();
        break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLMesh>(vertices, indices, materialIndex);
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanShader>();
        break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL:
        return std::make_shared<OpenGLMesh>(vertices, indices, materialIndex);
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanShader>();
        break;
#elif __APPLE__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL: break;
      case GraphicsApi::Metal:
        // return std::make_shared<MetalShader>();
        break;
      case GraphicsApi::Vulkan: break;
#endif
    }

    return nullptr;
  }

  std::shared_ptr<IMesh>
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

        Vertex vert({ x, y, z },
                    { x, y, z },
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

    return CreateMesh(vertices, indices, 0);
  }

  std::shared_ptr<IMesh>
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
    return CreateMesh(vertices, indices, 0);
  }

  std::shared_ptr<IMesh>
  MeshFactory::CreateUnitQuad()
  {
    return CreateMesh({ Vertex(glm::vec3(-0.5f, -0.5f, 0.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec2(0.0f, 0.0f)), // Bottom-left
                        Vertex(glm::vec3(0.5f, -0.5f, 0.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec2(1.0f, 0.0f)), // Bottom-right
                        Vertex(glm::vec3(0.5f, 0.5f, 0.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec2(1.0f, 1.0f)), // Top-right
                        Vertex(glm::vec3(-0.5f, 0.5f, 0.0f),
                               glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec2(0.0f, 1.0f)) },
                      { 0, 1, 2, 2, 3, 0 },
                      0);
  }
}