#include "ShaderRegistry.h"
#include "Core/Asset/Database/AssetComponents.h"
#include <fmt/format.h>

namespace Dwarf
{
  ShaderRegistry::ShaderRegistry(std::shared_ptr<IDwarfLogger>   logger,
                                 std::shared_ptr<IShaderFactory> shaderFactory)
    : mLogger(logger)
    , mShaderFactory(shaderFactory)
  {
    mLogger->LogDebug(Log("ShaderRegistry created", "ShaderRegistry"));
  }

  ShaderRegistry::~ShaderRegistry()
  {
    mLogger->LogDebug(Log("ShaderRegistry destroyed", "ShaderRegistry"));
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

    auto it = mShaders.find(hash);
    if (it != mShaders.end())
    {

      mLogger->LogDebug(
        Log(fmt::format("Shader program already compiled and found"),
            "ShaderRegistry"));
      return it->second; // Return existing shader program
    }

    mLogger->LogDebug(
      Log(fmt::format("Shader program not present yet. Building it now."),
          "ShaderRegistry"));

    mShaders[hash] = mShaderFactory->CreateShader(std::move(shaderSources));

    return mShaders[hash];
  }
}