#include "SceneSettingsFactory.h"

#include "Core/Scene/Settings/SceneSettings.h"
#include <memory>

namespace Dwarf
{
  std::unique_ptr<ISceneSettings>
  SceneSettingsFactory::Create(nlohmann::json serializedSettings)
  {
    return std::make_unique<SceneSettings>(serializedSettings);
  }

  std::unique_ptr<ISceneSettings>
  SceneSettingsFactory::Create()
  {
    return std::make_unique<SceneSettings>();
  }
} // namespace Dwarf