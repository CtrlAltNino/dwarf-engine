#include "SceneProperties.h"

namespace Dwarf
{
  SceneProperties::SceneProperties(
    std::shared_ptr<AssetReference<SceneAsset>> asset,
    std::string                                 name,
    std::shared_ptr<ISceneSettings>             settings)
    : m_Asset(asset)
    , m_Name(name)
    , m_Settings(settings)
  {
  }

  nlohmann::json
  SceneProperties::Serialize() const
  {
    nlohmann::json json;

    json["Settings"] = m_Settings->Serialize();

    return json;
  }

  std::string
  SceneProperties::GetName() const
  {
    return m_Name;
  }

  std::shared_ptr<AssetReference<SceneAsset>>
  SceneProperties::GetAsset() const
  {
    return m_Asset;
  }

  std::shared_ptr<ISceneSettings>
  SceneProperties::GetSettings() const
  {
    return m_Settings;
  }
} // namespace Dwarf