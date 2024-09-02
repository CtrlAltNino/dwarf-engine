#pragma once

#include "ISceneSettingsFactory.h"

namespace Dwarf
{
  class SceneSettingsFactory : public ISceneSettingsFactory
  {
  public:
    ~SceneSettingsFactory() override = default;

    std::shared_ptr<ISceneSettings>
    Create(nlohmann::json serializedSettings) override;

    std::shared_ptr<ISceneSettings>
    Create() override;
  };
} // namespace Dwarf