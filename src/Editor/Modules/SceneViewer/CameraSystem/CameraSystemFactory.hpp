#pragma once

#include "ICameraSystemFactory.hpp"
#include "Input/IInputManager.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class CameraSystemFactory : public ICameraSystemFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>  mLogger;
    std::shared_ptr<IInputManager> mInputManager;

  public:
    CameraSystemFactory(std::shared_ptr<IDwarfLogger>  logger,
                        std::shared_ptr<IInputManager> inputManager);

    ~CameraSystemFactory() override;

    auto
    Create() -> std::shared_ptr<ICameraSystem> override;
  };
}