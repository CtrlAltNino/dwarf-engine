#include "ShaderRegistry.h"
#include "Core/Asset/Database/AssetComponents.h"

namespace Dwarf
{
  ShaderRegistry::ShaderRegistry(std::shared_ptr<IDwarfLogger>   logger,
                                 std::shared_ptr<IShaderFactory> shaderFactory)
    : m_Logger(logger)
    , m_ShaderFactory(shaderFactory)
  {
    m_Logger->LogDebug(Log("ShaderRegistry created", "ShaderRegistry"));
  }

  ShaderRegistry::~ShaderRegistry()
  {
    m_Logger->LogDebug(Log("ShaderRegistry destroyed", "ShaderRegistry"));
  }

  std::shared_ptr<IShader>
  ShaderRegistry::GetOrCreate(
    std::unique_ptr<IShaderSourceCollection> shaderSources)
  {
    // Hashing the shader sources
    std::hash<std::string> hasher;
    std::string            combinedSource = "";

    for (auto& shaderSource : shaderSources->GetShaderSources())
    {
      std::string sourceString = "";

      switch (shaderSource->GetType())
      {
        case Dwarf::ASSET_TYPE::VERTEX_SHADER:
          sourceString =
            ((VertexShaderAsset&)shaderSource->GetAsset()).GetFileContent();
          break;
        case Dwarf::ASSET_TYPE::FRAGMENT_SHADER:
          sourceString =
            ((FragmentShaderAsset&)shaderSource->GetAsset()).GetFileContent();
          break;
        case Dwarf::ASSET_TYPE::GEOMETRY_SHADER:
          sourceString =
            ((GeometryShaderAsset&)shaderSource->GetAsset()).GetFileContent();
          break;
        case Dwarf::ASSET_TYPE::TESC_SHADER:
          sourceString =
            ((TessellationControlShaderAsset&)shaderSource->GetAsset())
              .GetFileContent();
          break;
        case Dwarf::ASSET_TYPE::TESE_SHADER:
          sourceString =
            ((TessellationEvaluationShaderAsset&)shaderSource->GetAsset())
              .GetFileContent();
          break;
        default: break;
      }
      combinedSource += sourceString;
    }

    size_t hash = hasher(combinedSource);

    auto it = m_Shaders.find(hash);
    if (it != m_Shaders.end())
    {

      m_Logger->LogDebug(
        Log(fmt::format("Shader program already compiled and found"),
            "ShaderRegistry"));
      return it->second; // Return existing shader program
    }

    m_Logger->LogDebug(
      Log(fmt::format("Shader program not present yet. Building it now."),
          "ShaderRegistry"));

    m_Shaders[hash] = m_ShaderFactory->CreateShader(std::move(shaderSources));

    return m_Shaders[hash];
  }
}