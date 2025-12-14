#include "pch.hpp"

#include "Camera.hpp"
#include "CameraFactory.hpp"

namespace Dwarf
{
  CameraFactory::CameraFactory(std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
  {
    mLogger->LogDebug(Log("CameraFactory created.", "CameraFactory"));
  }

  CameraFactory::~CameraFactory()
  {
    mLogger->LogDebug(Log("CameraFactory destroyed.", "CameraFactory"));
  }

  auto
  CameraFactory::Create() const -> std::unique_ptr<ICamera>
  {
    return std::make_unique<Camera>(mLogger);
  }

  auto
  CameraFactory::Create(CameraProperties properties) const
    -> std::unique_ptr<ICamera>
  {
    return std::make_unique<Camera>(mLogger, properties);
  }

  auto
  CameraFactory::Create(const nlohmann::json& serializedCameraData) const
    -> std::unique_ptr<ICamera>
  {
    return std::make_unique<Camera>(mLogger, serializedCameraData);
  }
}