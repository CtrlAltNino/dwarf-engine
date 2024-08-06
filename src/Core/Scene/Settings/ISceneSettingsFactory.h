#pragma once

#include "ISceneSettings.h"

namespace Dwarf
{
  class ISceneSettingsFactory
  {
  public:
    virtual ~ISceneSettingsFactory() = default;

    virtual std::shared_ptr<ISceneSettings>
    Create(nlohmann::json serializedSettings) = 0;

    virtual std::shared_ptr<ISceneSettings>
    Create() = 0;
  };
}