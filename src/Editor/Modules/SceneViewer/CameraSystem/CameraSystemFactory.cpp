#include "CameraSystem.hpp"
#include "pch.hpp"

#include "CameraSystemFactory.hpp"

namespace Dwarf
{
  CameraSystemFactory::CameraSystemFactory(
    std::shared_ptr<IDwarfLogger>  logger,
    std::shared_ptr<IInputManager> inputManager)
    : mLogger(std::move(logger))
    , mInputManager(std::move(inputManager))
  {
    mLogger->LogDebug(
      Log("CameraSystemFactory created", "CameraSystemFactory"));
  }

  CameraSystemFactory::~CameraSystemFactory()
  {
    mLogger->LogDebug(
      Log("CameraSystemFactory destroyed", "CameraSystemFactory"));
  }

  auto
  CameraSystemFactory::Create() -> std::shared_ptr<ICameraSystem>
  {
    return std::make_shared<CameraSystem>(mLogger, mInputManager);
  }
}