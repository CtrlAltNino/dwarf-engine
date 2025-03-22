#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Scene/Properties/IScenePropertiesFactory.h"
#include "ISceneFactory.h"
#include "Logging/IDwarfLogger.h"
#include "Utilities/FileHandler/IFileHandler.h"

namespace Dwarf
{
  /// @brief Factory for creating scenes.
  class SceneFactory : public ISceneFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>            mLogger;
    std::shared_ptr<IScenePropertiesFactory> mScenePropertiesFactory;
    std::shared_ptr<IAssetDatabase>          mAssetDatabase;
    std::shared_ptr<IFileHandler>            mFileHandler;

  public:
    SceneFactory(
      std::shared_ptr<IDwarfLogger>            logger,
      std::shared_ptr<IScenePropertiesFactory> scenePropertiesFactory,
      std::shared_ptr<IAssetDatabase>          assetDatabase,
      std::shared_ptr<IFileHandler>            fileHandler);
    virtual ~SceneFactory() override;

    [[nodiscard]] auto
    FromAsset(IAssetReference& sceneAsset) const
      -> std::unique_ptr<IScene> override;

    [[nodiscard]] auto
    CreateDefaultScene() const -> std::unique_ptr<IScene> override;
  };
} // namespace Dwarf