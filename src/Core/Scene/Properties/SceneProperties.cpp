#include "pch.hpp"

#include "SceneProperties.hpp"

namespace Dwarf
{
  using std::string;

  SceneProperties::SceneProperties(std::optional<UUID>             assetId,
                                   std::string_view                name,
                                   std::unique_ptr<ISceneSettings> settings)
    : mAssetId(std::move(assetId))
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

  void
  SceneProperties::SetName(const std::string& sceneName)
  {
    mName = sceneName;
  }

  auto
  SceneProperties::GetAssetId() const -> const std::optional<UUID>&
  {
    return mAssetId;
  }

  void
  SceneProperties::SetAssetId(const UUID& id)
  {
    mAssetId = id;
  }

  auto
  SceneProperties::GetSettings() const -> ISceneSettings&
  {
    return *mSettings;
  }
} // namespace Dwarf