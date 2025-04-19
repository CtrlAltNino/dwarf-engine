#include "pch.hpp"

#include "Core/Asset/Database/AssetComponents.hpp"
#include "ShaderRegistry.hpp"

namespace Dwarf
{
  ShaderRegistry::ShaderRegistry(std::shared_ptr<IDwarfLogger>   logger,
                                 std::shared_ptr<IShaderFactory> shaderFactory)
    : mLogger(std::move(logger))
    , mShaderFactory(std::move(shaderFactory))
  {
    mLogger->LogDebug(Log("ShaderRegistry created", "ShaderRegistry"));
  }

  ShaderRegistry::~ShaderRegistry()
  {
    mLogger->LogDebug(Log("ShaderRegistry destroyed", "ShaderRegistry"));
  }

  auto
  ShaderRegistry::GetOrCreate(std::unique_ptr<IShaderSourceCollection>
                                shaderSources) -> std::shared_ptr<IShader>
  {
    // Hashing the shader sources
    std::hash<std::string> hasher;
    std::string            combinedSource;

    for (auto& shaderSource : shaderSources->GetShaderSources())
    {
      std::string sourceString;

      switch (shaderSource->GetType())
      {
        case Dwarf::ASSET_TYPE::VERTEX_SHADER:
          sourceString =
            (dynamic_cast<VertexShaderAsset&>(shaderSource->GetAsset()))
              .GetFileContent();
          break;
        case Dwarf::ASSET_TYPE::FRAGMENT_SHADER:
          sourceString =
            (dynamic_cast<FragmentShaderAsset&>(shaderSource->GetAsset()))
              .GetFileContent();
          break;
        case Dwarf::ASSET_TYPE::GEOMETRY_SHADER:
          sourceString =
            (dynamic_cast<GeometryShaderAsset&>(shaderSource->GetAsset()))
              .GetFileContent();
          break;
        case Dwarf::ASSET_TYPE::TESC_SHADER:
          sourceString = (dynamic_cast<TessellationControlShaderAsset&>(
                            shaderSource->GetAsset()))
                           .GetFileContent();
          break;
        case Dwarf::ASSET_TYPE::TESE_SHADER:
          sourceString = (dynamic_cast<TessellationEvaluationShaderAsset&>(
                            shaderSource->GetAsset()))
                           .GetFileContent();
          break;
        default: break;
      }
      combinedSource += sourceString;
    }

    size_t hash = hasher(combinedSource);

    auto iterator = mShaders.find(hash);
    if (iterator != mShaders.end())
    {

      mLogger->LogDebug(
        Log(fmt::format("Shader program already compiled and found"),
            "ShaderRegistry"));
      return iterator->second; // Return existing shader program
    }

    mLogger->LogDebug(
      Log(fmt::format("Shader program not present yet. Building it now."),
          "ShaderRegistry"));

    mShaders[hash] = mShaderFactory->Create(std::move(shaderSources));
    mShaders[hash]->Compile();

    return mShaders[hash];
  }
}