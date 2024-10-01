#include "SceneProperties.h"

namespace Dwarf
{
  SceneProperties::SceneProperties(std::optional<UUID>             assetID,
                                   const std::string&              name,
                                   std::unique_ptr<ISceneSettings> settings)
    : m_AssetID(assetID)
    , m_Name(name)
    , m_Settings(std::move(settings))
  {
  }

  nlohmann::json
  SceneProperties::Serialize()
  {
    return m_Settings->Serialize();
  }

  std::string
  SceneProperties::GetName() const
  {
    return m_Name;
  }

  const std::optional<UUID>&
  SceneProperties::GetAssetID() const
  {
    return m_AssetID;
  }

  ISceneSettings&
  SceneProperties::GetSettings() const
  {
    return *m_Settings;
  }
} // namespace Dwarf