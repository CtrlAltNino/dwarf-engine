#include "dpch.h"
#include "Mesh.h"

#include "Core/Rendering/Renderer.h"

#ifdef _WIN32
// #include "Platform/Direct3D12/D3D12Mesh.h"
#include "Platform/OpenGL/OpenGLMesh.h"
// #include "Platform/Vulkan/VulkanMesh.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLMesh.h"
// #include "Platform/Vulkan/VulkanMesh.h"
#elif __APPLE__
// #include "Platform/Metal/MetalMesh.h"
#endif

namespace Dwarf
{
	Ref<Mesh> Mesh::s_GridMesh = nullptr;
	Ref<Mesh> Mesh::s_UnitSphere = nullptr;
	Ref<Mesh> Mesh::Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex)
	{
		switch (Renderer::GetAPI())
		{
#ifdef _WIN32
		case GraphicsApi::D3D12:
			// return CreateRef<D3D12Mesh>(spec);
			break;
		case GraphicsApi::Metal:
			break;
		case GraphicsApi::OpenGL:
			return CreateRef<OpenGLMesh>(vertices, indices, materialIndex);
			break;
		case GraphicsApi::Vulkan:
			// return CreateRef<VulkanFramebuffer>(spec);
			break;
#elif __linux__
		case GraphicsApi::D3D12:
			break;
		case GraphicsApi::Metal:
			break;
		case GraphicsApi::OpenGL:
			return CreateRef<OpenGLMesh>(vertices, indices, materialIndex);
			break;
		case GraphicsApi::Vulkan:
			// return CreateRef<VulkanFramebuffer>(spec);
			break;
#elif __APPLE__
		case GraphicsApi::D3D12:
			break;
		case GraphicsApi::Metal:
			// return CreateRef<MetalFramebuffer>(spec);
			break;
		case GraphicsApi::OpenGL:
			break;
		case GraphicsApi::Vulkan:
			break;
#endif
		}
		return nullptr;
	}

	Ref<Mesh> Mesh::GenerateUnitSphere(int stacks, int slices)
	{
		std::vector<Vertex> vertices = std::vector<Vertex>();
		std::vector<unsigned int> indices = std::vector<unsigned int>();
		for (int i = 0; i <= stacks; ++i)
		{
			float phi = glm::pi<float>() * static_cast<float>(i) / static_cast<float>(stacks);
			for (int j = 0; j <= slices; ++j)
			{
				float theta = 2.0f * glm::pi<float>() * static_cast<float>(j) / static_cast<float>(slices);

				float x = std::sin(phi) * std::cos(theta);
				float y = std::cos(phi);
				float z = std::sin(phi) * std::sin(theta);

				Vertex vert;
				vert.Position = {x, y, z};
				vert.Normal = {x, y, z};

				float u = static_cast<float>(j) / static_cast<float>(slices);
				float v = static_cast<float>(i) / static_cast<float>(stacks);
				// uvs.push_back(glm::vec2(u, v));
				vert.UV = {u, v};

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

		return Mesh::Create(vertices, indices, 0);
	}

	void Mesh::Init()
	{
		s_GridMesh = Mesh::Create(
			{{{-50, 0, 50}, {0, 1, 0}},
			 {{50, 0, 50}, {0, 1, 0}},
			 {{50, 0, -50}, {0, 1, 0}},
			 {{-50, 0, -50}, {0, 1, 0}}},
			{0, 1, 2, 2, 3, 0}, 0);
		s_GridMesh->SetupMesh();
		s_UnitSphere = Mesh::GenerateUnitSphere(20, 20);
		s_UnitSphere->SetupMesh();
	}
}