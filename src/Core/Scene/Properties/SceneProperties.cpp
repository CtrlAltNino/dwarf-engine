#include "SceneProperties.h"

#include <utility>

namespace Dwarf
{
  using std::string;

  SceneProperties::SceneProperties(std::optional<UUID>             assetID,
                                   std::string_view                name,
                                   std::unique_ptr<ISceneSettings> settings)
    : mAssetID(std::move(assetID))
    , mName(name)
    , mSettings(std::move(settings))
  {
  }

  auto
  SceneProperties::Serialize() -> nlohmann::json
  {
    return mSettings->Serialize();
  }

  auto
  SceneProperties::GetName() const -> std::string
  {
    return mName;
  }

  auto
  SceneProperties::GetAssetID() const -> const std::optional<UUID>&
  {
    return mAssetID;
  }

  auto
  SceneProperties::GetSettings() const -> ISceneSettings&
  {
    return *mSettings;
  }
} // namespace Dwarf