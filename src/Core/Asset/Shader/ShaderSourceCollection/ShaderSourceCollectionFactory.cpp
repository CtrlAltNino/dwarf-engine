#include "pch.hpp"

#include "Core/Asset/AssetReference/IAssetReference.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollection.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/ShaderSourceCollection.hpp"
#include "Platform/OpenGL/OpenGLUtilities.hpp"
#include "ShaderSourceCollectionFactory.hpp"

namespace Dwarf
{
  ShaderSourceCollectionFactory::ShaderSourceCollectionFactory(
    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> assetDatabase,
    GraphicsApi                                                 graphicsApi)
    : mAssetDatabase(assetDatabase)
    , mGraphicsApi(graphicsApi)
  {
  }

  auto
  ShaderSourceCollectionFactory::CreateDefaultShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetDefaultShaderPath() / "default.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetDefaultShaderPath() / "default.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateErrorShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetErrorShaderPath() / "error.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetErrorShaderPath() / "error.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateIdShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetIdShaderPath() / "id.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetIdShaderPath() / "id.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateGridShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetFullScreenQuadShaderPath() /
          "fullscreen_quad.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetGridShaderPath() / "grid_postprocess.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateExposureScalingShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetFullScreenQuadShaderPath() /
          "fullscreen_quad.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetExposureScalingShaderPath() /
          "exposure_scaling.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateReinhardTonemapShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetFullScreenQuadShaderPath() /
          "fullscreen_quad.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetReinhardTonemapShaderPath() /
          "reinhard_tonemap.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateAgxTonemapShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetFullScreenQuadShaderPath() /
          "fullscreen_quad.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetAgxTonemapShaderPath() / "agx_tonemap.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateAcesTonemapShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetFullScreenQuadShaderPath() /
          "fullscreen_quad.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetAcesTonemapShaderPath() / "aces_tonemap.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateUncharted2TonemapShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetFullScreenQuadShaderPath() /
          "fullscreen_quad.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetUncharted2TonemapShaderPath() /
          "uncharted2_tonemap.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreatePreviewShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetPreviewShaderPath() / "preview.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetPreviewShaderPath() / "preview.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreatePbrShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetPbrShaderPath() / "pbr.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetPbrShaderPath() / "pbr.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateUnlitShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetUnlitShaderPath() / "unlit.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetUnlitShaderPath() / "unlit.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateColorSkyboxShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetSkyboxShaderPath() / "skybox.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetSkyboxShaderPath() / "skybox_color.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateHdriSkyboxShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetSkyboxShaderPath() / "skybox.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetSkyboxShaderPath() / "skybox_hdri.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateCubemapConversionShaderSourceCollection()
    -> std::unique_ptr<IShaderSourceCollection>
  {
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    switch (mGraphicsApi)
    {
      case GraphicsApi::OpenGL:
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetSkyboxShaderPath() / "cubemap_conversion.vert"));
        shaderSources.emplace_back(mAssetDatabase.get()->Retrieve(
          OpenGLUtilities::GetSkyboxShaderPath() / "cubemap_conversion.frag"));
        break;
      case GraphicsApi::Vulkan:
      case GraphicsApi::D3D12:
      default: throw std::runtime_error("Unsupported Graphics API.");
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateShaderSourceCollection(
    const nlohmann::json& serializedShaderSourceCollection)
    -> std::unique_ptr<IShaderSourceCollection>
  {
    // Extracting shader sources from the JSON object.
    std::vector<std::unique_ptr<IAssetReference>> shaderSources = {};

    if (serializedShaderSourceCollection.contains("VertexShader") &&
        !serializedShaderSourceCollection["VertexShader"]
           .get<std::string>()
           .empty())
    {
      UUID vertexShaderId =
        UUID(serializedShaderSourceCollection["VertexShader"]);

      shaderSources.emplace_back(
        std::move(mAssetDatabase.get()->Retrieve(vertexShaderId)));
    }

    if (serializedShaderSourceCollection.contains("FragmentShader") &&
        !serializedShaderSourceCollection["FragmentShader"]
           .get<std::string>()
           .empty())
    {
      UUID fragmentShaderId =
        UUID(serializedShaderSourceCollection["FragmentShader"]);

      shaderSources.emplace_back(
        mAssetDatabase.get()->Retrieve(fragmentShaderId));
    }

    if (serializedShaderSourceCollection.contains("GeometryShader") &&
        !serializedShaderSourceCollection["GeometryShader"]
           .get<std::string>()
           .empty())
    {
      UUID geometryShaderId =
        UUID(serializedShaderSourceCollection["GeometryShader"]);

      shaderSources.emplace_back(
        mAssetDatabase.get()->Retrieve(geometryShaderId));
    }

    if (serializedShaderSourceCollection.contains(
          "TessellationControlShader") &&
        !serializedShaderSourceCollection["TessellationControlShader"]
           .get<std::string>()
           .empty())
    {
      UUID tessellationControlShaderId =
        UUID(serializedShaderSourceCollection["TessellationControlShader"]);

      shaderSources.emplace_back(
        mAssetDatabase.get()->Retrieve(tessellationControlShaderId));
    }

    if (serializedShaderSourceCollection.contains(
          "TessellationEvaluationShader") &&
        !serializedShaderSourceCollection["TessellationEvaluationShader"]
           .get<std::string>()
           .empty())
    {
      UUID tessellationEvaluationShaderId =
        UUID(serializedShaderSourceCollection["TessellationEvaluationShader"]);

      shaderSources.emplace_back(
        mAssetDatabase.get()->Retrieve(tessellationEvaluationShaderId));
    }

    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }

  auto
  ShaderSourceCollectionFactory::CreateShaderSourceCollection(
    std::vector<std::unique_ptr<IAssetReference>>& shaderSources)
    -> std::unique_ptr<IShaderSourceCollection>
  {
    return std::make_unique<ShaderSourceCollection>(shaderSources);
  }
}