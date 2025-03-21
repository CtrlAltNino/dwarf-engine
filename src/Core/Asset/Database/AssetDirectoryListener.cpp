#include "AssetDirectoryListener.h"
#include <fmt/format.h>

namespace Dwarf
{
  AssetDirectoryListener::AssetDirectoryListener(
    const AssetDirectoryPath&     assetDirectoryPath,
    std::shared_ptr<IDwarfLogger> logger)
    : mLogger(std::move(logger))
  {
    mLogger->LogDebug(
      Log("Creating AssetDirectoryListener", "AssetDirectoryListener"));
    mWatchID = mFileWatcher.addWatch(assetDirectoryPath.t.string(), this, true);
    mFileWatcher.watch();
    mLogger->LogDebug(
      Log("AssetDirectoryListener created", "AssetDirectoryListener"));
  }

  AssetDirectoryListener::~AssetDirectoryListener()
  {
    mLogger->LogDebug(
      Log("Destroying AssetDirectoryListener", "AssetDirectoryListener"));
    mFileWatcher.removeWatch(mWatchID);
    mLogger->LogDebug(
      Log("AssetDirectoryListener destroyed", "AssetDirectoryListener"));
  }

  void
  AssetDirectoryListener::registerAddFileCallback(
    std::function<void(const std::string& dir, const std::string& filename)>
      callback)
  {
    mAddFileCallbacks.push_back(callback);
  }

  void
  AssetDirectoryListener::registerDeleteFileCallback(
    std::function<void(const std::string& dir, const std::string& filename)>
      callback)
  {
    mDeleteFileCallbacks.push_back(callback);
  }

  void
  AssetDirectoryListener::registerModifyFileCallback(
    std::function<void(const std::string& dir, const std::string& filename)>
      callback)
  {
    mModifyFileCallbacks.push_back(callback);
  }

  void
  AssetDirectoryListener::registerMoveFileCallback(
    std::function<void(const std::string& dir,
                       const std::string& filename,
                       std::string        oldFilename)> callback)
  {
    mMoveFileCallbacks.push_back(callback);
  }

  void
  AssetDirectoryListener::handleFileAction(efsw::WatchID      watchid,
                                           const std::string& dir,
                                           const std::string& filename,
                                           efsw::Action       action,
                                           std::string        oldFilename)
  {

    switch (action)
    {
      case efsw::Actions::Add:
        {
          mLogger->LogInfo(
            Log(fmt::format("File with name {} has been added in directory {}",
                            filename,
                            dir),
                "AssetDirectoryListener"));

          for (const auto& callback : mAddFileCallbacks)
          {
            callback(dir, filename);
          }
        }
        break;
      case efsw::Actions::Delete:
        {
          mLogger->LogInfo(Log(
            fmt::format("File with name {} has been deleted in directory {}",
                        filename,
                        dir),
            "AssetDirectoryListener"));
          for (const auto& callback : mDeleteFileCallbacks)
          {
            callback(dir, filename);
          }
        }
        break;
      case efsw::Actions::Modified:
        {
          mLogger->LogInfo(Log(
            fmt::format("File with name {} has been modified in directory {}",
                        filename,
                        dir),
            "AssetDirectoryListener"));
          for (const auto& callback : mModifyFileCallbacks)
          {
            callback(dir, filename);
          }
        }
        break;
      case efsw::Actions::Moved:
        {
          mLogger->LogInfo(
            Log(fmt::format(
                  "File with name {} has been moved to directory {} from {}",
                  filename,
                  dir,
                  oldFilename),
                "AssetDirectoryListener"));
          for (const auto& callback : mMoveFileCallbacks)
          {
            callback(dir, filename, oldFilename);
          }
        }
        break;
      default:
        mLogger->LogError(Log("Invalid EFSW case", "AssetDirectoryListener"));
    }
  }
}