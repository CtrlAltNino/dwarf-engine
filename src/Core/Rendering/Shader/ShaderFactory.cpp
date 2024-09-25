#include "ShaderFactory.h"
#include <fmt/format.h>
#include <memory>

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
      shaderParameterCollectionFactory)
    : m_GraphicsApi(graphicsApi)
    , m_Logger(logger)
    , m_ShaderSourceCollectionFactory(shaderSourceCollectionFactory)
    , m_ShaderParameterCollectionFactory(shaderParameterCollectionFactory)
  {
    m_Logger->LogInfo(Log("ShaderFactory created", "ShaderFactory"));
  }

  ShaderFactory::~ShaderFactory()
  {
    m_Logger->LogInfo(Log("ShaderFactory destroyed", "ShaderFactory"));
  }

  std::unique_ptr<IShader>
  ShaderFactory::CreateDefaultShader()
  {
    m_Logger->LogInfo(Log("Creating default shader", "ShaderFactory"));
    // TODO: Implement the default shader creation.
    return CreateShader(
      m_ShaderSourceCollectionFactory->CreateDefaultShaderSourceCollection());
  }

  std::unique_ptr<IShader>
  ShaderFactory::CreateErrorShader()
  {
    m_Logger->LogInfo(Log("Creating error shader", "ShaderFactory"));
    return CreateShader(
      m_ShaderSourceCollectionFactory->CreateErrorShaderSourceCollection());
  }

  std::unique_ptr<IShader>
  ShaderFactory::CreateShader(const nlohmann::json& shaderJson)
  {
    m_Logger->LogInfo(
      Log(fmt::format("Creating shader from JSON:\n{}", shaderJson.dump(2)),
          "ShaderFactory"));
    // Extracting shader sources from the JSON object.
    // ShaderSourceCollection shaderSources;

    // if (shaderJson.contains("vertexShader"))
    // {
    //   std::shared_ptr<UUID> vertexShaderId =
    //     std::make_shared<UUID>(shaderJson["vertexShader"]);

    //   shaderSources.t.emplace_back(
    //     m_AssetDatabase->Retrieve<VertexShaderAsset>(vertexShaderId));
    // }

    // if (shaderJson.contains("fragmentShader"))
    // {
    //   std::shared_ptr<UUID> fragmentShaderId =
    //     std::make_shared<UUID>(shaderJson["fragmentShader"]);

    //   shaderSources.t.emplace_back(
    //     m_AssetDatabase->Retrieve<FragmentShaderAsset>(fragmentShaderId));
    // }

    // if (shaderJson.contains("geometryShader"))
    // {
    //   std::shared_ptr<UUID> geometryShaderId =
    //     std::make_shared<UUID>(shaderJson["geometryShader"]);

    //   shaderSources.t.emplace_back(
    //     m_AssetDatabase->Retrieve<GeometryShaderAsset>(geometryShaderId));
    // }

    // if (shaderJson.contains("tessellationControlShader"))
    // {
    //   std::shared_ptr<UUID> tessellationControlShaderId =
    //     std::make_shared<UUID>(shaderJson["tessellationControlShader"]);

    //   shaderSources.t.emplace_back(
    //     m_AssetDatabase->Retrieve<TessellationControlShaderAsset>(
    //       tessellationControlShaderId));
    // }

    // if (shaderJson.contains("tessellationEvaluationShader"))
    // {
    //   std::shared_ptr<UUID> tessellationEvaluationShaderId =
    //     std::make_shared<UUID>(shaderJson["tessellationEvaluationShader"]);

    //   shaderSources.t.emplace_back(
    //     m_AssetDatabase->Retrieve<TessellationEvaluationShaderAsset>(
    //       tessellationEvaluationShaderId));
    // }

    // Calling the CreateShader method that takes a ShaderSourceCollection.
    return CreateShader(
      m_ShaderSourceCollectionFactory->CreateShaderSourceCollection(
        shaderJson));
  }

  std::unique_ptr<IShader>
  ShaderFactory::CreateShader(
    std::unique_ptr<IShaderSourceCollection> shaderSources)
  {
    m_Logger->LogInfo(Log("Creating shader from sources", "ShaderFactory"));
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
          m_Logger);
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
          m_Logger);
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
} // namespace Dwarf