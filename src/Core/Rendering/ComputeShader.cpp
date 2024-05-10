#include "Core/Rendering/ComputeShader.h"
#include "Core/Rendering/Renderer.h"

#ifdef _WIN32
// #include "Platform/D3D12/D3D12Shader.h"
#include "Platform/OpenGL/OpenGLComputeShader.h"
// #include "Platform/Vulkan/VulkanShader.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLComputeShader.h"
// #include "Platform/Vulkan/VulkanShader.h"
#elif __APPLE__
// #include "Platform/Metal/MetalShader.h"
#endif
#include "Shader Parameters/BooleanShaderParameter.h"
#include "Shader Parameters/IntegerShaderParameter.h"
#include "Shader Parameters/UnsignedIntegerShaderParameter.h"
#include "Shader Parameters/FloatShaderParameter.h"
#include "Shader Parameters/Vec2ShaderParameter.h"
#include "Shader Parameters/Vec3ShaderParameter.h"
#include "Shader Parameters/Vec4ShaderParameter.h"
#include "Shader Parameters/Tex2DShaderParameter.h"

namespace Dwarf
{

  std::shared_ptr<ComputeShader> ComputeShader::s_PropagationShader = nullptr;
  std::shared_ptr<ComputeShader> ComputeShader::s_FinalizationShader = nullptr;

  std::shared_ptr<ComputeShader>
  ComputeShader::Create()
  {
    switch (Renderer::GetAPI())
    {
      using enum GraphicsApi;
#ifdef _WIN32
      case D3D12: break;
      case Metal: break;
      case OpenGL: return std::make_shared<OpenGLComputeShader>(); break;
      case Vulkan: break;
#elif __linux__
      case D3D12: break;
      case Metal: break;
      case OpenGL: return std::make_shared<OpenGLComputeShader>(); break;
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
  ComputeShader::GetOutlineShaderPath()
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

  ComputeShader::ComputeShader() = default;
  ComputeShader::~ComputeShader() = default;

  void
  ComputeShader::Init()
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
        s_FinalizationShader = OpenGLComputeShader::CreateFinalizationShader();
        s_PropagationShader = OpenGLComputeShader::CreatePropagationShader();
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
        s_FinalizationShader = OpenGLComputeShader::CreateFinalizationShader();
        s_PropagationShader = OpenGLComputeShader::CreatePropagationShader();
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

    s_PropagationShader->Compile();
    s_FinalizationShader->Compile();
  }

  std::shared_ptr<IShaderParameter>
  ComputeShader::CreateComputeShaderParameter(ShaderParameterType type)
  {
    switch (type)
    {
      using enum ShaderParameterType;
      case BOOLEAN: return std::make_shared<BooleanShaderParameter>(); break;
      case INTEGER: return std::make_shared<IntegerShaderParameter>(); break;
      case FLOAT: return std::make_shared<FloatShaderParameter>(); break;
      case VEC2: return std::make_shared<Vec2ShaderParameter>(); break;
      case VEC3: return std::make_shared<Vec3ShaderParameter>(); break;
      case VEC4: return std::make_shared<Vec4ShaderParameter>(); break;
      case TEX2D: return std::make_shared<Tex2DShaderParameter>(); break;
      default: return nullptr;
    }
  }

  template<>
  void
  ComputeShader::SetParameter<bool>(std::string_view identifier, bool value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() ==
         ShaderParameterType::BOOLEAN))
    {
      std::dynamic_pointer_cast<BooleanShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<BooleanShaderParameter>(value);
    }
  }

  template<>
  void
  ComputeShader::SetParameter<int>(std::string_view identifier, int value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() ==
         ShaderParameterType::INTEGER))
    {
      std::dynamic_pointer_cast<IntegerShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<IntegerShaderParameter>(value);
    }
  }

  template<>
  void
  ComputeShader::SetParameter<unsigned int>(std::string_view identifier,
                                            unsigned int     value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() ==
         ShaderParameterType::UNSIGNED_INTEGER))
    {
      std::dynamic_pointer_cast<UnsignedIntegerShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<UnsignedIntegerShaderParameter>(value);
    }
  }

  template<>
  void
  ComputeShader::SetParameter<float>(std::string_view identifier, float value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters.find(identifier)->second->GetType() ==
         ShaderParameterType::FLOAT))
    {
      std::dynamic_pointer_cast<FloatShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<FloatShaderParameter>(value);
    }
  }

  template<>
  void
  ComputeShader::SetParameter<glm::vec2>(std::string_view identifier,
                                         glm::vec2        value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() ==
         ShaderParameterType::VEC2))
    {
      std::dynamic_pointer_cast<Vec2ShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<Vec2ShaderParameter>(value);
    }
  }

  template<>
  void
  ComputeShader::SetParameter<glm::vec3>(std::string_view identifier,
                                         glm::vec3        value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() ==
         ShaderParameterType::VEC3))
    {
      std::dynamic_pointer_cast<Vec3ShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<Vec3ShaderParameter>(value);
    }
  }

  template<>
  void
  ComputeShader::SetParameter<glm::vec4>(std::string_view identifier,
                                         glm::vec4        value)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() ==
         ShaderParameterType::VEC4))
    {
      std::dynamic_pointer_cast<Vec4ShaderParameter>(
        m_Parameters[std::string(identifier)])
        ->m_Value = value;
    }
    else
    {
      m_Parameters[std::string(identifier)] =
        std::make_shared<Vec4ShaderParameter>(value);
    }
  }

  void
  ComputeShader::SetParameter(std::string_view     identifier,
                              std::shared_ptr<UID> value,
                              ShaderParameterType  type)
  {
    if (m_Parameters.contains(identifier) &&
        (m_Parameters[std::string(identifier)]->GetType() == type))
    {
      switch (type)
      {
        using enum ShaderParameterType;
        case TEX2D:
          std::dynamic_pointer_cast<Tex2DShaderParameter>(
            m_Parameters[std::string(identifier)])
            ->m_Value = value;
          break;
        case BOOLEAN: break;
        case INTEGER: break;
        case UNSIGNED_INTEGER: break;
        case FLOAT: break;
        case VEC2: break;
        case VEC3: break;
        case VEC4: break;
      }
    }
    else
    {
      switch (type)
      {
        using enum ShaderParameterType;
        case TEX2D:
          m_Parameters[std::string(identifier)] =
            std::make_shared<Tex2DShaderParameter>(value);
          break;
        case BOOLEAN: break;
        case INTEGER: break;
        case UNSIGNED_INTEGER: break;
        case FLOAT: break;
        case VEC2: break;
        case VEC3: break;
        case VEC4: break;
      }
    }
  }
}