#include "SceneSettingsFactory.h"

#include "Core/Scene/Settings/SceneSettings.h"

namespace Dwarf
{
  std::unique_ptr<ISceneSettings>
  SceneSettingsFactory::Create(nlohmann::json serializedSettings)
  {
    return std::make_unique<SceneSettings>(SceneSettings(serializedSettings));
  }

  std::unique_ptr<ISceneSettings>
  SceneSettingsFactory::Create()
  {
    return std::make_unique<SceneSettings>(SceneSettings());
  }
} // namespace Dwarf