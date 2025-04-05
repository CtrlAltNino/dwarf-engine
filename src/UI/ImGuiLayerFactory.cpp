#include "pch.h"

#include "ImGuiLayerFactory.h"
#include "Platform/OpenGL/OpenGLImGuiLayer.h"

namespace Dwarf
{
  ImGuiLayerFactory::ImGuiLayerFactory(std::shared_ptr<IDwarfLogger> logger,
                                       GraphicsApi                   api,
                                       const ImGuiIniFilePath& iniFilePath)
    : mLogger(std::move(logger))
    , mApi(api)
    , mIniFilePath(iniFilePath)
  {
    mLogger->LogDebug(Log("ImGuiLayerFactory created", "ImGuiLayerFactory"));
  }

  ImGuiLayerFactory::~ImGuiLayerFactory()
  {
    mLogger->LogDebug(Log("ImGuiLayerFactory destroyed", "ImGuiLayerFactory"));
  }

  auto
  ImGuiLayerFactory::Create() const -> std::unique_ptr<IImGuiLayer>
  {
    mLogger->LogDebug(Log("Creating ImGuiLayer", "ImGuiLayerFactory"));

    switch (mApi)
    {
      using enum GraphicsApi;
      case None:
        mLogger->LogError(Log("Graphics API is not set", "ImGuiLayerFactory"));
        throw std::runtime_error("Graphics API is not set");
      case OpenGL:
        return std::make_unique<OpenGLImGuiLayer>(mLogger, mIniFilePath);
      case Vulkan:
        mLogger->LogError(
          Log("Vulkan API has not been implemented yet.", "ImGuiLayerFactory"));
        throw std::runtime_error("Vulkan API has not been implemented yet.");
      case GraphicsApi::D3D12:
#if _WIN32
        mLogger->LogError(
          Log("D3D12 API has not been implemented yet.", "ImGuiLayerFactory"));
        throw std::runtime_error("D3D12 API has not been implemented yet.");
#elif __linux__
        mLogger->LogError(
          Log("D3D12 API has not been implemented yet.", "ImGuiLayerFactory"));
        throw std::runtime_error("D3D12 API has not been implemented yet.");
#endif
    }

    mLogger->LogError(Log("Failed to create ImGuiLayer.", "ImGuiLayerFactory"));

    return nullptr;
  }
} // namespace Dwarf