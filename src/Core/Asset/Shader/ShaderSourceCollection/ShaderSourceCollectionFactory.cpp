#include "ShaderSourceCollectionFactory.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Shader/ShaderSourceCollection/ShaderSourceCollection.h"
#include <iostream>
#include <memory>
#include <vector>

namespace Dwarf
{
  ShaderSourceCollectionFactory::ShaderSourceCollectionFactory(
    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> assetDatabase,
    GraphicsApi                                                 graphicsApi)
    : m_AssetDatabase(assetDatabase)
    , m_GraphicsApi(graphicsApi)
  {
  }

  std::unique_ptr<IShaderSourceCollection>
  ShaderSourceCollectionFactory::CreateDefaultShaderSourceCollection()
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};
    std::cout << "1" << std::endl;

    switch (m_GraphicsApi)
    {
      case GraphicsApi::OpenGL:
        std::cout << "2" << std::endl;
        // Check if asset database is nullptr
        std::cout << "AssetDatabase: " << (m_AssetDatabase.get() == nullptr)
                  << std::endl;
        shaderSources.emplace_back(m_AssetDatabase.get()->Retrieve(
          "data/engine/shaders/default/opengl/default.vert"));
        std::cout << "3" << std::endl;
        shaderSources.emplace_back(m_AssetDatabase.get()->Retrieve(
          "data/engine/shaders/default/opengl/default.frag"));
        std::cout << "4" << std::endl;
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      case GraphicsApi::Metal:
        throw std::runtime_error("Graphics API not supported yet.");
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    std::cout << "5" << std::endl;

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  std::unique_ptr<IShaderSourceCollection>
  ShaderSourceCollectionFactory::CreateShaderSourceCollection(
    const nlohmann::json& serializedShaderSourceCollection)
  {
    // Extracting shader sources from the JSON object.
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    if (serializedShaderSourceCollection.contains("vertexShader"))
    {
      UUID vertexShaderId =
        UUID(serializedShaderSourceCollection["vertexShader"]);

      shaderSources.emplace_back(
        std::move(m_AssetDatabase.get()->Retrieve(vertexShaderId)));
    }

    if (serializedShaderSourceCollection.contains("fragmentShader"))
    {
      UUID fragmentShaderId =
        UUID(serializedShaderSourceCollection["fragmentShader"]);

      shaderSources.emplace_back(
        m_AssetDatabase.get()->Retrieve(fragmentShaderId));
    }

    if (serializedShaderSourceCollection.contains("geometryShader"))
    {
      UUID geometryShaderId =
        UUID(serializedShaderSourceCollection["geometryShader"]);

      shaderSources.emplace_back(
        m_AssetDatabase.get()->Retrieve(geometryShaderId));
    }

    if (serializedShaderSourceCollection.contains("tessellationControlShader"))
    {
      UUID tessellationControlShaderId =
        UUID(serializedShaderSourceCollection["tessellationControlShader"]);

      shaderSources.emplace_back(
        m_AssetDatabase.get()->Retrieve(tessellationControlShaderId));
    }

    if (serializedShaderSourceCollection.contains(
          "tessellationEvaluationShader"))
    {
      UUID tessellationEvaluationShaderId =
        UUID(serializedShaderSourceCollection["tessellationEvaluationShader"]);

      shaderSources.emplace_back(
        m_AssetDatabase.get()->Retrieve(tessellationEvaluationShaderId));
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }
}