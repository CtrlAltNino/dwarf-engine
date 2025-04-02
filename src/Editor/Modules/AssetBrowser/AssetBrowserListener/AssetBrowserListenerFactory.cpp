#include "pch.h"

#include "AssetBrowserListener.h"
#include "AssetBrowserListenerFactory.h"

namespace Dwarf
{
  AssetBrowserListenerFactory::AssetBrowserListenerFactory(
    AssetDirectoryPath            assetDirectoryPath,
    std::shared_ptr<IDwarfLogger> logger)
    : mAssetDirectoryPath(assetDirectoryPath)
    , mLogger(std::move(logger))
  {
    mLogger->LogDebug(Log("AssetBrowserListenerFactory created",
                          "AssetBrowserListenerFactory"));
  }

  AssetBrowserListenerFactory::~AssetBrowserListenerFactory()
  {
    mLogger->LogDebug(Log("AssetBrowserListenerFactory destroyed",
                          "AssetBrowserListenerFactory"));
  }

  auto
  AssetBrowserListenerFactory::Create(std::function<void()> callback) const
    -> std::unique_ptr<IAssetBrowserListener>
  {
    return std::make_unique<AssetBrowserListener>(
      mAssetDirectoryPath, mLogger, callback);
  }
}