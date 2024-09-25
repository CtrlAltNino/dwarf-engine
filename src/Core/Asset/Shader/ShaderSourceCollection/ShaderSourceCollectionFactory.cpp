#include "ShaderSourceCollectionFactory.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.h"
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

    switch (m_GraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(m_AssetDatabase.get()->Retrieve(
          "data/engine/shaders/default/opengl/default.vert"));
        shaderSources.emplace_back(m_AssetDatabase.get()->Retrieve(
          "data/engine/shaders/default/opengl/default.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      case GraphicsApi::Metal:
        throw std::runtime_error("Graphics API not supported yet.");
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  std::unique_ptr<IShaderSourceCollection>
  ShaderSourceCollectionFactory::CreateErrorShaderSourceCollection()
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (m_GraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(m_AssetDatabase.get()->Retrieve(
          "data/engine/shaders/error/opengl/error.vert"));
        shaderSources.emplace_back(m_AssetDatabase.get()->Retrieve(
          "data/engine/shaders/error/opengl/error.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      case GraphicsApi::Metal:
        throw std::runtime_error("Graphics API not supported yet.");
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  std::unique_ptr<IShaderSourceCollection>
  ShaderSourceCollectionFactory::CreateShaderSourceCollection(
    const nlohmann::json& serializedShaderSourceCollection)
  {
    // Extracting shader sources from the JSON object.
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    if (serializedShaderSourceCollection.contains("vertexShader") &&
        !serializedShaderSourceCollection["vertexShader"]
           .get<std::string>()
           .empty())
    {
      UUID vertexShaderId =
        UUID(serializedShaderSourceCollection["vertexShader"]);

      shaderSources.emplace_back(
        std::move(m_AssetDatabase.get()->Retrieve(vertexShaderId)));
    }

    if (serializedShaderSourceCollection.contains("fragmentShader") &&
        !serializedShaderSourceCollection["fragmentShader"]
           .get<std::string>()
           .empty())
    {
      UUID fragmentShaderId =
        UUID(serializedShaderSourceCollection["fragmentShader"]);

      shaderSources.emplace_back(
        m_AssetDatabase.get()->Retrieve(fragmentShaderId));
    }

    if (serializedShaderSourceCollection.contains("geometryShader") &&
        !serializedShaderSourceCollection["geometryShader"]
           .get<std::string>()
           .empty())
    {
      UUID geometryShaderId =
        UUID(serializedShaderSourceCollection["geometryShader"]);

      shaderSources.emplace_back(
        m_AssetDatabase.get()->Retrieve(geometryShaderId));
    }

    if (serializedShaderSourceCollection.contains(
          "tessellationControlShader") &&
        !serializedShaderSourceCollection["tessellationControlShader"]
           .get<std::string>()
           .empty())
    {
      UUID tessellationControlShaderId =
        UUID(serializedShaderSourceCollection["tessellationControlShader"]);

      shaderSources.emplace_back(
        m_AssetDatabase.get()->Retrieve(tessellationControlShaderId));
    }

    if (serializedShaderSourceCollection.contains(
          "tessellationEvaluationShader") &&
        !serializedShaderSourceCollection["tessellationEvaluationShader"]
           .get<std::string>()
           .empty())
    {
      UUID tessellationEvaluationShaderId =
        UUID(serializedShaderSourceCollection["tessellationEvaluationShader"]);

      shaderSources.emplace_back(
        m_AssetDatabase.get()->Retrieve(tessellationEvaluationShaderId));
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }
}