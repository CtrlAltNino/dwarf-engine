#include "Core/Rendering/Shader Parameters/UnsignedIntegerShaderParameter.h"
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

  std::shared_ptr<Shader> Shader::s_DefaultShader = nullptr;
  std::shared_ptr<Shader> Shader::s_ErrorShader = nullptr;
  std::shared_ptr<Shader> Shader::s_GridShader = nullptr;
  std::shared_ptr<Shader> Shader::s_PreviewShader = nullptr;
  std::shared_ptr<Shader> Shader::s_IdShader = nullptr;
  std::shared_ptr<Shader> Shader::s_WhiteShader = nullptr;

  std::shared_ptr<Shader>
  Shader::Create()
  {
    switch (Renderer::GetAPI())
    {
      using enum GraphicsApi;
#ifdef _WIN32
      case D3D12: break;
      case Metal: break;
      case OpenGL: return std::make_shared<OpenGLShader>(); break;
      case Vulkan: break;
#elif __linux__
      case D3D12: break;
      case Metal: break;
      case OpenGL: return std::make_shared<OpenGLShader>(); break;
      case Vulkan: break;
#elif __APPLE__
      case D3D12: break;
      case Metal:
        // return std::make_shared<MetalShader>();
        break;
      case OpenGL: break;
      case Vulkan: break;
#endif
      default: return nullptr;
    }

    return nullptr;
  }

  std::filesystem::path
  Shader::GetDefaultShaderPath()
  {
    switch (Renderer::GetAPI())
    {
      using enum GraphicsApi;
      case D3D12: return "data/engine/shaders/default/d3d12/"; break;
      case Metal: return "data/engine/shaders/default/metal/"; break;
      case OpenGL: return "data/engine/shaders/default/opengl/"; break;
      case Vulkan: return "data/engine/shaders/default/vulkan/"; break;
    }

    return "";
  }

  std::filesystem::path
  Shader::GetErrorShaderPath()
  {
    switch (Renderer::GetAPI())
    {
      using enum GraphicsApi;
      case D3D12: return "data/engine/shaders/error/d3d12/"; break;
      case Metal: return "data/engine/shaders/error/metal/"; break;
      case OpenGL: return "data/engine/shaders/error/opengl/"; break;
      case Vulkan: return "data/engine/shaders/error/vulkan/"; break;
    }

    return "";
  }

  std::filesystem::path
  Shader::GetGridShaderPath()
  {
    switch (Renderer::GetAPI())
    {
      using enum GraphicsApi;
      case D3D12: return "data/engine/shaders/grid/d3d12/"; break;
      case Metal: return "data/engine/shaders/grid/metal/"; break;
      case OpenGL: return "data/engine/shaders/grid/opengl/"; break;
      case Vulkan: return "data/engine/shaders/grid/vulkan/"; break;
    }

    return "";
  }

  std::filesystem::path
  Shader::GetPreviewShaderPath()
  {
    switch (Renderer::GetAPI())
    {
      using enum GraphicsApi;
      case D3D12: return "data/engine/shaders/preview/d3d12/"; break;
      case Metal: return "data/engine/shaders/preview/metal/"; break;
      case OpenGL: return "data/engine/shaders/preview/opengl/"; break;
      case Vulkan: return "data/engine/shaders/preview/vulkan/"; break;
    }

    return "";
  }

  std::filesystem::path
  Shader::GetIdShaderPath()
  {
    switch (Renderer::GetAPI())
    {
      using enum GraphicsApi;
      case D3D12: return "data/engine/shaders/id/d3d12/"; break;
      case Metal: return "data/engine/shaders/id/metal/"; break;
      case OpenGL: return "data/engine/shaders/id/opengl/"; break;
      case Vulkan: return "data/engine/shaders/id/vulkan/"; break;
    }

    return "";
  }

  std::filesystem::path
  Shader::GetOutlineShaderPath()
  {
    switch (Renderer::GetAPI())
    {
      using enum GraphicsApi;
      case D3D12: return "data/engine/shaders/outline/d3d12/"; break;
      case Metal: return "data/engine/shaders/outline/metal/"; break;
      case OpenGL: return "data/engine/shaders/outline/opengl/"; break;
      case Vulkan: return "data/engine/shaders/outline/vulkan/"; break;
    }

    return "";
  }

  Shader::Shader() = default;
  Shader::~Shader() = default;

  void
  Shader::Init()
  {
    switch (Renderer::GetAPI())
    {
#ifdef _WIN32
      case GraphicsApi::D3D12:
        // s_DefaultShader = D3D12Shader::CreateDefaultShader();
        // s_ErrorShader = D3D12Shader::CreateErrorShader();
        // s_GridShader = D3D12Shader::CreateGridShader();
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::OpenGL:
        s_DefaultShader = OpenGLShader::CreateDefaultShader();
        s_ErrorShader = OpenGLShader::CreateErrorShader();
        s_GridShader = OpenGLShader::CreateGridShader();
        s_PreviewShader = OpenGLShader::CreatePreviewShader();
        s_IdShader = OpenGLShader::CreateIdShader();
        s_WhiteShader = OpenGLShader::CreateWhiteShader();
        break;
      case GraphicsApi::Vulkan:
        // s_DefaultShader = VulkanShader::CreateDefaultShader();
        // s_ErrorShader = VulkanShader::CreateErrorShader();
        // s_GridShader = VulkanShader::CreateGridShader();
        break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::OpenGL:
        s_DefaultShader = OpenGLShader::CreateDefaultShader();
        s_ErrorShader = OpenGLShader::CreateErrorShader();
        s_GridShader = OpenGLShader::CreateGridShader();
        s_PreviewShader = OpenGLShader::CreatePreviewShader();
        s_IdShader = OpenGLShader::CreateIdShader();
        s_WhiteShader = OpenGLShader::CreateWhiteShader();
        break;
      case GraphicsApi::Vulkan: break;
#elif __APPLE__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::Metal:
        // s_DefaultShader = MetalShader::CreateDefaultShader();
        // s_ErrorShader = MetalShader::CreateErrorShader();
        // s_GridShader = MetalShader::CreateGridShader();
        break;
      case GraphicsApi::OpenGL: break;
      case GraphicsApi::Vulkan: break;
#endif
    }

    s_DefaultShader->Compile();
    s_ErrorShader->Compile();
    s_GridShader->Compile();
  }

  std::shared_ptr<IShaderParameter>
  Shader::CreateShaderParameter(ShaderParameterType type)
  {
    switch (type)
    {
      using enum ShaderParameterType;
      case BOOLEAN: return std::make_shared<BooleanShaderParameter>(); break;
      case INTEGER: return std::make_shared<IntegerShaderParameter>(); break;
      case UNSIGNED_INTEGER:
        return std::make_shared<UnsignedIntegerShaderParameter>();
        break;
      case FLOAT: return std::make_shared<FloatShaderParameter>(); break;
      case VEC2: return std::make_shared<Vec2ShaderParameter>(); break;
      case VEC3: return std::make_shared<Vec3ShaderParameter>(); break;
      case VEC4: return std::make_shared<Vec4ShaderParameter>(); break;
      case TEX2D: return std::make_shared<Tex2DShaderParameter>(); break;
      default: return nullptr;
    }
  }
}