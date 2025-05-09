#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "IAssetBrowserListenerFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class AssetBrowserListenerFactory : public IAssetBrowserListenerFactory
  {
    AssetDirectoryPath            mAssetDirectoryPath;
    std::shared_ptr<IDwarfLogger> mLogger;

  public:
    AssetBrowserListenerFactory(AssetDirectoryPath assetDirectoryPath,
                                std::shared_ptr<IDwarfLogger> logger);
    ~AssetBrowserListenerFactory() override;

    [[nodiscard]] auto
    Create(std::function<void()> callback) const
      -> std::unique_ptr<IAssetBrowserListener> override;
  };
}