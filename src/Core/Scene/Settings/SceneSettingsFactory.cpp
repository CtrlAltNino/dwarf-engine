#include "SceneSettingsFactory.h"

#include "Core/Scene/Settings/SceneSettings.h"

namespace Dwarf
{
  std::shared_ptr<ISceneSettings>
  SceneSettingsFactory::Create(nlohmann::json serializedSettings)
  {
    return std::make_shared<SceneSettings>(serializedSettings);
  }

  std::shared_ptr<ISceneSettings>
  SceneSettingsFactory::Create()
  {
    return std::make_shared<SceneSettings>();
  }
} // namespace Dwarf