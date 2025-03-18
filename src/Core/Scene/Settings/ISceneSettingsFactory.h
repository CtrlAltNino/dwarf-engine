#pragma once

#include "ISceneSettings.h"
#include <memory>
#include <nlohmann/json.hpp>

namespace Dwarf
{
  class ISceneSettingsFactory
  {
  public:
    virtual ~ISceneSettingsFactory() = default;

    virtual std::unique_ptr<ISceneSettings>
    Create(nlohmann::json serializedSettings) = 0;

    virtual std::unique_ptr<ISceneSettings>
    Create() = 0;
  };
}