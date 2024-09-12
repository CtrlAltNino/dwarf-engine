#include "SceneProperties.h"

namespace Dwarf
{
  SceneProperties::SceneProperties(
    std::unique_ptr<IAssetReference<SceneAsset>> asset,
    const std::string&                           name,
    std::unique_ptr<ISceneSettings>              settings)
    : m_Asset(std::move(asset))
    , m_Name(name)
    , m_Settings(std::move(settings))
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

  IAssetReference<SceneAsset>&
  SceneProperties::GetAsset() const
  {
    return *m_Asset;
  }

  ISceneSettings&
  SceneProperties::GetSettings() const
  {
    return *m_Settings;
  }
} // namespace Dwarf