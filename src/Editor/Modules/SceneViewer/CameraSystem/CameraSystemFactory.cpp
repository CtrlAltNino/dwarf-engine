#include "pch.hpp"

#include "CameraSystem.hpp"
#include "CameraSystemFactory.hpp"

namespace Dwarf
{
  CameraSystemFactory::CameraSystemFactory(
    std::shared_ptr<IDwarfLogger>  logger,
    std::shared_ptr<IInputManager> inputManager,
    std::shared_ptr<ILoadedScene>  loadedScene)
    : mLogger(std::move(logger))
    , mInputManager(std::move(inputManager))
    , mLoadedScene(std::move(loadedScene))
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
    return std::make_shared<CameraSystem>(mLogger, mInputManager, mLoadedScene);
  }

  auto
  CameraSystemFactory::Create(nlohmann::json serializedCameraSystem)
    -> std::shared_ptr<ICameraSystem>
  {
    return std::make_shared<CameraSystem>(
      serializedCameraSystem, mLogger, mInputManager, mLoadedScene);
  }
}