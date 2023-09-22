#include "dpch.h"
#include "Mesh.h"

#include "Core/Rendering/Renderer.h"

#ifdef _WIN32
//#include "Platform/Direct3D12/D3D12Mesh.h"
#include "Platform/OpenGL/OpenGLMesh.h"
//#include "Platform/Vulkan/VulkanMesh.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLMesh.h"
//#include "Platform/Vulkan/VulkanMesh.h"
#elif __APPLE__
//#include "Platform/Metal/MetalMesh.h"
#endif

namespace Dwarf {
	Ref<Mesh> Mesh::s_GridMesh = nullptr;
    Ref<Mesh> Mesh::Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex)
	{
		switch (Renderer::GetAPI())
		{
#ifdef _WIN32
			case GraphicsApi::D3D12:
					//return CreateRef<D3D12Mesh>(spec);
				break;
			case GraphicsApi::Metal: break;
			case GraphicsApi::OpenGL:
					return CreateRef<OpenGLMesh>(vertices, indices, materialIndex);
				break;
			case GraphicsApi::Vulkan:
					//return CreateRef<VulkanFramebuffer>(spec);
				break;
#elif __linux__
			case GraphicsApi::D3D12: break;
			case GraphicsApi::Metal: break;
			case GraphicsApi::OpenGL:
					return CreateRef<OpenGLFramebuffer>(spec);
				break;
			case GraphicsApi::Vulkan:
					//return CreateRef<VulkanFramebuffer>(spec);
				break;
#elif __APPLE__
			case GraphicsApi::D3D12: break;
			case GraphicsApi::Metal:
					//return CreateRef<MetalFramebuffer>(spec);
				break;
			case GraphicsApi::OpenGL: break;
			case GraphicsApi::Vulkan: break;
#endif
		}
		return nullptr;
	}

	void Mesh::Init(){
		s_GridMesh = Mesh::Create(
			{
				{{-50,0,50}, {0,1,0}},
				{{50,0,50}, {0,1,0}},
				{{50,0,-50}, {0,1,0}},
				{{-50,0,-50}, {0,1,0}}
			},
			{0, 1, 2, 2, 3, 0}, 0
		);
	}
}