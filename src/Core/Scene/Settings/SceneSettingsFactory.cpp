#include "pch.hpp"

#include "Core/Scene/Settings/SceneSettings.hpp"
#include "SceneSettingsFactory.hpp"

namespace Dwarf
{
  SceneSettingsFactory::SceneSettingsFactory(
    std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
  {
    mLogger->LogDebug(
      Log("SceneSettingsFactory created.", "SceneSettingsFactory"));
  }

  SceneSettingsFactory::~SceneSettingsFactory()
  {
    mLogger->LogDebug(
      Log("SceneSettingsFactory destroyed.", "SceneSettingsFactory"));
  }

  auto
  SceneSettingsFactory::Create(nlohmann::json serializedSettings) const
    -> std::unique_ptr<ISceneSettings>
  {
    return std::make_unique<SceneSettings>(serializedSettings);
  }

  auto
  SceneSettingsFactory::Create() const -> std::unique_ptr<ISceneSettings>
  {
    return std::make_unique<SceneSettings>();
  }
} // namespace Dwarf