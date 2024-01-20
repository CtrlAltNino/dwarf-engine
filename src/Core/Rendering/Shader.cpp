#include "dpch.h"
#include <string>

#include "Core/Rendering/Shader.h"
#include "Core/Rendering/Renderer.h"

#ifdef _WIN32
// #include "Platform/D3D12/D3D12Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
// #include "Platform/Vulkan/VulkanShader.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLShader.h"
// #include "Platform/Vulkan/VulkanShader.h"
#elif __APPLE__
// #include "Platform/Metal/MetalShader.h"
#endif
#include "Shader Parameters/BooleanShaderParameter.h"
#include "Shader Parameters/IntegerShaderParameter.h"
#include "Shader Parameters/FloatShaderParameter.h"
#include "Shader Parameters/Vec2ShaderParameter.h"
#include "Shader Parameters/Vec3ShaderParameter.h"
#include "Shader Parameters/Vec4ShaderParameter.h"
#include "Shader Parameters/Tex2DShaderParameter.h"

namespace Dwarf
{

	Ref<Shader> Shader::s_DefaultShader = nullptr;
	Ref<Shader> Shader::s_ErrorShader = nullptr;
	Ref<Shader> Shader::s_GridShader = nullptr;

	Ref<Shader> Shader::Create()
	{
		switch (Renderer::GetAPI())
		{
#ifdef _WIN32
		case GraphicsApi::D3D12:
			// return CreateRef<D3D12Shader>(D3D12Shader());
			break;
		case GraphicsApi::Metal:
			break;
		case GraphicsApi::OpenGL:
			return CreateRef<OpenGLShader>(OpenGLShader());
			break;
		case GraphicsApi::Vulkan:
			// return CreateRef<VulkanShader>(VulkanShader());
			break;
#elif __linux__
		case GraphicsApi::D3D12:
			break;
		case GraphicsApi::Metal:
			break;
		case GraphicsApi::OpenGL:
			return CreateRef<OpenGLShader>(OpenGLShader());
			break;
		case GraphicsApi::Vulkan:
			// return CreateRef<VulkanShader>(VulkanShader());
			break;
#elif __APPLE__
		case GraphicsApi::D3D12:
			break;
		case GraphicsApi::Metal:
			// return CreateRef<MetalShader>(MetalShader());
			break;
		case GraphicsApi::OpenGL:
			break;
		case GraphicsApi::Vulkan:
			break;
#endif
		default:
			return nullptr;
		}

		return nullptr;
	}

	std::filesystem::path Shader::GetDefaultShaderPath()
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsApi::D3D12:
			return "data/engine/shaders/default/d3d12/";
			break;
		case GraphicsApi::Metal:
			return "data/engine/shaders/default/metal/";
			break;
		case GraphicsApi::OpenGL:
			return "data/engine/shaders/default/opengl/";
			break;
		case GraphicsApi::Vulkan:
			return "data/engine/shaders/default/vulkan/";
			break;
		}

		return "";
	}

	std::filesystem::path Shader::GetErrorShaderPath()
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsApi::D3D12:
			return "data/engine/shaders/error/d3d12/";
			break;
		case GraphicsApi::Metal:
			return "data/engine/shaders/error/metal/";
			break;
		case GraphicsApi::OpenGL:
			return "data/engine/shaders/error/opengl/";
			break;
		case GraphicsApi::Vulkan:
			return "data/engine/shaders/error/vulkan/";
			break;
		}

		return "";
	}

	std::filesystem::path Shader::GetGridShaderPath()
	{
		switch (Renderer::GetAPI())
		{
		case GraphicsApi::D3D12:
			return "data/engine/shaders/grid/d3d12/";
			break;
		case GraphicsApi::Metal:
			return "data/engine/shaders/grid/metal/";
			break;
		case GraphicsApi::OpenGL:
			return "data/engine/shaders/grid/opengl/";
			break;
		case GraphicsApi::Vulkan:
			return "data/engine/shaders/grid/vulkan/";
			break;
		}

		return "";
	}

	Shader::Shader() {}
	Shader::~Shader() {}

	void Shader::Init()
	{
		switch (Renderer::GetAPI())
		{
#ifdef _WIN32
		case GraphicsApi::D3D12:
			// s_DefaultShader = D3D12Shader::CreateDefaultShader();
			// s_ErrorShader = D3D12Shader::CreateErrorShader();
			// s_GridShader = D3D12Shader::CreateGridShader();
			break;
		case GraphicsApi::Metal:
			break;
		case GraphicsApi::OpenGL:
			s_DefaultShader = OpenGLShader::CreateDefaultShader();
			s_ErrorShader = OpenGLShader::CreateErrorShader();
			s_GridShader = OpenGLShader::CreateGridShader();
			break;
		case GraphicsApi::Vulkan:
			// s_DefaultShader = VulkanShader::CreateDefaultShader();
			// s_ErrorShader = VulkanShader::CreateErrorShader();
			// s_GridShader = VulkanShader::CreateGridShader();
			break;
#elif __linux__
		case GraphicsApi::D3D12:
			break;
		case GraphicsApi::Metal:
			break;
		case GraphicsApi::OpenGL:
			s_DefaultShader = OpenGLShader::CreateDefaultShader();
			s_ErrorShader = OpenGLShader::CreateErrorShader();
			s_GridShader = OpenGLShader::CreateGridShader();
			break;
		case GraphicsApi::Vulkan:
			// s_DefaultShader = VulkanShader::CreateDefaultShader();
			// s_ErrorShader = VulkanShader::CreateErrorShader();
			// s_GridShader = VulkanShader::CreateGridShader();
			break;
#elif __APPLE__
		case GraphicsApi::D3D12:
			break;
		case GraphicsApi::Metal:
			// s_DefaultShader = MetalShader::CreateDefaultShader();
			// s_ErrorShader = MetalShader::CreateErrorShader();
			// s_GridShader = MetalShader::CreateGridShader();
			break;
		case GraphicsApi::OpenGL:
			break;
		case GraphicsApi::Vulkan:
			break;
#endif
		}

		s_DefaultShader->Compile();
		s_ErrorShader->Compile();
		s_GridShader->Compile();
	}

	Ref<IShaderParameter> Shader::CreateShaderParameter(ShaderParameterType type)
	{
		switch (type)
		{
		case BOOLEAN:
			return CreateRef<BooleanShaderParameter>(BooleanShaderParameter());
			break;
		case INTEGER:
			return CreateRef<IntegerShaderParameter>(IntegerShaderParameter());
			break;
		case FLOAT:
			return CreateRef<FloatShaderParameter>(FloatShaderParameter());
			break;
		case VEC2:
			return CreateRef<Vec2ShaderParameter>(Vec2ShaderParameter());
			break;
		case VEC3:
			return CreateRef<Vec3ShaderParameter>(Vec3ShaderParameter());
			break;
		case VEC4:
			return CreateRef<Vec4ShaderParameter>(Vec4ShaderParameter());
			break;
		case TEX2D:
			return CreateRef<Tex2DShaderParameter>(Tex2DShaderParameter());
			break;
		default:
			return nullptr;
		}
	}
}