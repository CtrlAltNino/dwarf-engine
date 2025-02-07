#include "ShaderFactory.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include <fmt/format.h>
#include <memory>
#include <stdexcept>

// Including the shader header files of the graphics API.
#if _WIN32
#include "Platform/OpenGL/OpenGLShader.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLShader.h"
#elif __APPLE__
// #include "Platform/Metal/MetalShader.h"
#endif

namespace Dwarf
{
  ShaderFactory::ShaderFactory(
    GraphicsApi                   graphicsApi,
    std::shared_ptr<IDwarfLogger> logger,
    std::shared_ptr<IShaderSourceCollectionFactory>
      shaderSourceCollectionFactory,
    std::shared_ptr<IShaderParameterCollectionFactory>
                                  shaderParameterCollectionFactory,
    std::shared_ptr<IVramTracker> vramTracker)
    : m_GraphicsApi(graphicsApi)
    , m_Logger(logger)
    , m_ShaderSourceCollectionFactory(shaderSourceCollectionFactory)
    , m_ShaderParameterCollectionFactory(shaderParameterCollectionFactory)
    , m_VramTracker(vramTracker)
  {
    m_Logger->LogDebug(Log("ShaderFactory created", "ShaderFactory"));
  }

  ShaderFactory::~ShaderFactory()
  {
    m_Logger->LogDebug(Log("ShaderFactory destroyed", "ShaderFactory"));
  }

  std::unique_ptr<IShader>
  ShaderFactory::CreateDefaultShader()
  {
    m_Logger->LogDebug(Log("Creating default shader", "ShaderFactory"));
    // TODO: Implement the default shader creation.
    return CreateShader(
      m_ShaderSourceCollectionFactory->CreateDefaultShaderSourceCollection());
  }

  std::unique_ptr<IShader>
  ShaderFactory::CreateErrorShader()
  {
    m_Logger->LogDebug(Log("Creating error shader", "ShaderFactory"));
    return CreateShader(
      m_ShaderSourceCollectionFactory->CreateErrorShaderSourceCollection());
  }

  std::unique_ptr<IShader>
  ShaderFactory::CreateShader(const nlohmann::json& shaderJson)
  {
    m_Logger->LogDebug(
      Log(fmt::format("Creating shader from JSON:\n{}", shaderJson.dump(2)),
          "ShaderFactory"));

    // Calling the CreateShader method that takes a ShaderSourceCollection.
    return CreateShader(
      m_ShaderSourceCollectionFactory->CreateShaderSourceCollection(
        shaderJson));
  }

  std::unique_ptr<IShader>
  ShaderFactory::CreateShader(
    std::unique_ptr<IShaderSourceCollection> shaderSources)
  {
    m_Logger->LogDebug(Log("Creating shader from sources", "ShaderFactory"));
    // Creating a shader based on the graphics API.
    switch (m_GraphicsApi)
    {
#if _WIN32
      case GraphicsApi::D3D12:
        // return std::make_shared<D3D12Shader>();
        break;
      case GraphicsApi::OpenGL:
        return std::make_unique<OpenGLShader>(
          std::move(shaderSources),
          m_ShaderParameterCollectionFactory,
          m_Logger,
          m_VramTracker);
        break;
      case GraphicsApi::Metal: break;
      case GraphicsApi::Vulkan:
        // return std::make_shared<VulkanShader>();
        break;
#elif __linux__
      case GraphicsApi::D3D12: break;
      case GraphicsApi::OpenGL:
        return std::make_unique<OpenGLShader>(
          std::move(shaderSources),
          m_ShaderParameterCollectionFactory,
          m_Logger,
          m_VramTracker);
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
      case GraphicsApi::None:
        m_Logger->LogError(Log("Graphics API is not set", "ShaderFactory"));
        std::runtime_error("Graphics API is not set");
        break;
    }

    return nullptr;
  }
} // namespace Dwarf