#include "ShaderSourceCollectionFactory.h"

namespace Dwarf
{
  ShaderSourceCollectionFactory::ShaderSourceCollectionFactory(
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : m_AssetDatabase(assetDatabase)
  {
  }

  ShaderSourceCollection
  ShaderSourceCollectionFactory::CreateShaderSourceCollection(
    nlohmann::json serializedShaderSourceCollection)
  {
    // Extracting shader sources from the JSON object.
    ShaderSourceCollection shaderSources;

    if (serializedShaderSourceCollection.contains("vertexShader"))
    {
      std::shared_ptr<UUID> vertexShaderId = std::make_shared<UUID>(
        serializedShaderSourceCollection["vertexShader"]);

      shaderSources.t.emplace_back(
        m_AssetDatabase->Retrieve<VertexShaderAsset>(vertexShaderId));
    }

    if (serializedShaderSourceCollection.contains("fragmentShader"))
    {
      std::shared_ptr<UUID> fragmentShaderId = std::make_shared<UUID>(
        serializedShaderSourceCollection["fragmentShader"]);

      shaderSources.t.emplace_back(
        m_AssetDatabase->Retrieve<FragmentShaderAsset>(fragmentShaderId));
    }

    if (serializedShaderSourceCollection.contains("geometryShader"))
    {
      std::shared_ptr<UUID> geometryShaderId = std::make_shared<UUID>(
        serializedShaderSourceCollection["geometryShader"]);

      shaderSources.t.emplace_back(
        m_AssetDatabase->Retrieve<GeometryShaderAsset>(geometryShaderId));
    }

    if (serializedShaderSourceCollection.contains("tessellationControlShader"))
    {
      std::shared_ptr<UUID> tessellationControlShaderId =
        std::make_shared<UUID>(
          serializedShaderSourceCollection["tessellationControlShader"]);

      shaderSources.t.emplace_back(
        m_AssetDatabase->Retrieve<TessellationControlShaderAsset>(
          tessellationControlShaderId));
    }

    if (serializedShaderSourceCollection.contains(
          "tessellationEvaluationShader"))
    {
      std::shared_ptr<UUID> tessellationEvaluationShaderId =
        std::make_shared<UUID>(
          serializedShaderSourceCollection["tessellationEvaluationShader"]);

      shaderSources.t.emplace_back(
        m_AssetDatabase->Retrieve<TessellationEvaluationShaderAsset>(
          tessellationEvaluationShaderId));
    }

    return shaderSources;
  }
}