#include "SceneProperties.h"

namespace Dwarf
{
  SceneProperties::SceneProperties(std::optional<UUID>             assetID,
                                   const std::string&              name,
                                   std::unique_ptr<ISceneSettings> settings)
    : mAssetID(assetID)
    , mName(name)
    , mSettings(std::move(settings))
  {
  }

  nlohmann::json
  SceneProperties::Serialize()
  {
    return mSettings->Serialize();
  }

  std::string
  SceneProperties::GetName() const
  {
    return mName;
  }

  const std::optional<UUID>&
  SceneProperties::GetAssetID() const
  {
    return mAssetID;
  }

  ISceneSettings&
  SceneProperties::GetSettings() const
  {
    return *mSettings;
  }
} // namespace Dwarf