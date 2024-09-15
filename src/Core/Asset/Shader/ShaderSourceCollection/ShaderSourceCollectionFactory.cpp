#include "ShaderSourceCollectionFactory.h"
#include "Core/Asset/Shader/ShaderSourceCollection/ShaderSourceCollection.h"
#include <memory>
#include <vector>

namespace Dwarf
{
  ShaderSourceCollectionFactory::ShaderSourceCollectionFactory(
    std::shared_ptr<IAssetDatabase> assetDatabase,
    GraphicsApi                     graphicsApi)
    : m_AssetDatabase(assetDatabase)
    , m_GraphicsApi(graphicsApi)
  {
  }

  std::unique_ptr<IShaderSourceCollection>
  ShaderSourceCollectionFactory::CreateDefaultShaderSourceCollection()
  {
    std::vector<ShaderSource> shaderSources = {};
    // TODO: Get default shader sources depending on the graphics API.
    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  std::unique_ptr<IShaderSourceCollection>
  ShaderSourceCollectionFactory::CreateShaderSourceCollection(
    const nlohmann::json& serializedShaderSourceCollection)
  {
    // Extracting shader sources from the JSON object.
    std::vector<ShaderSource> shaderSources = {};

    if (serializedShaderSourceCollection.contains("vertexShader"))
    {
      UUID vertexShaderId =
        UUID(serializedShaderSourceCollection["vertexShader"]);

      shaderSources.emplace_back(std::move(
        m_AssetDatabase->Retrieve<VertexShaderAsset>(vertexShaderId)));
    }

    if (serializedShaderSourceCollection.contains("fragmentShader"))
    {
      UUID fragmentShaderId =
        UUID(serializedShaderSourceCollection["fragmentShader"]);

      shaderSources.emplace_back(
        m_AssetDatabase->Retrieve<FragmentShaderAsset>(fragmentShaderId));
    }

    if (serializedShaderSourceCollection.contains("geometryShader"))
    {
      UUID geometryShaderId =
        UUID(serializedShaderSourceCollection["geometryShader"]);

      shaderSources.emplace_back(
        m_AssetDatabase->Retrieve<GeometryShaderAsset>(geometryShaderId));
    }

    if (serializedShaderSourceCollection.contains("tessellationControlShader"))
    {
      UUID tessellationControlShaderId =
        UUID(serializedShaderSourceCollection["tessellationControlShader"]);

      shaderSources.emplace_back(
        m_AssetDatabase->Retrieve<TessellationControlShaderAsset>(
          tessellationControlShaderId));
    }

    if (serializedShaderSourceCollection.contains(
          "tessellationEvaluationShader"))
    {
      UUID tessellationEvaluationShaderId =
        UUID(serializedShaderSourceCollection["tessellationEvaluationShader"]);

      shaderSources.emplace_back(
        m_AssetDatabase->Retrieve<TessellationEvaluationShaderAsset>(
          tessellationEvaluationShaderId));
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }
}