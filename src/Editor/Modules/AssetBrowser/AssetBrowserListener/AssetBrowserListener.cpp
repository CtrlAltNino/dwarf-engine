#include "pch.h"

#include "AssetBrowserListener.h"

namespace Dwarf
{
  AssetBrowserListener::AssetBrowserListener(
    const AssetDirectoryPath&     assetDirectoryPath,
    std::shared_ptr<IDwarfLogger> logger,
    std::function<void()>         callback)
    : mLogger(std::move(logger))
    , mCallback(std::move(callback))
  {
    mLogger->LogDebug(
      Log("Creating AssetDirectoryListener", "AssetDirectoryListener"));
    mWatchID = mFileWatcher.addWatch(assetDirectoryPath.t.string(), this, true);
    mFileWatcher.watch();
    mLogger->LogDebug(
      Log("AssetDirectoryListener created", "AssetDirectoryListener"));
  }

  AssetBrowserListener::~AssetBrowserListener()
  {
    mLogger->LogDebug(
      Log("Destroying AssetDirectoryListener", "AssetDirectoryListener"));
    mFileWatcher.removeWatch(mWatchID);
    mLogger->LogDebug(
      Log("AssetDirectoryListener destroyed", "AssetDirectoryListener"));
  }

  void
  AssetBrowserListener::handleFileAction(efsw::WatchID      watchid,
                                         const std::string& dir,
                                         const std::string& filename,
                                         efsw::Action       action,
                                         std::string        oldFilename)
  {
    mCallback();
  }
}