#include "AssetDirectoryListener.h"

namespace Dwarf
{
  AssetDirectoryListener::AssetDirectoryListener(
    const AssetDirectoryPath& assetDirectoryPath)
  {
    mWatchID = mFileWatcher.addWatch(assetDirectoryPath.t.string(), this, true);
    // mWatchID = mFileWatcher.addWatch(
    //   Shader::GetDefaultShaderPath().string(), this, true);

    // Start watching asynchronously the directories
    mFileWatcher.watch();
  }

  AssetDirectoryListener::~AssetDirectoryListener()
  {
    mFileWatcher.removeWatch(mWatchID);
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
        std::cout << "DIR (" << dir << ") FILE (" << filename
                  << ") has event Added\n";

        for (const auto& callback : mAddFileCallbacks)
        {
          callback(dir, filename);
        }
        break;
      case efsw::Actions::Delete:
        std::cout << "DIR (" << dir << ") FILE (" << filename
                  << ") has event Delete\n";
        for (const auto& callback : mDeleteFileCallbacks)
        {
          callback(dir, filename);
        }
        break;
      case efsw::Actions::Modified:
        std::cout << "DIR (" << dir << ") FILE (" << filename
                  << ") has event Modified\n";
        for (const auto& callback : mModifyFileCallbacks)
        {
          callback(dir, filename);
        }
        break;
      case efsw::Actions::Moved:
        std::cout << "DIR (" << dir << ") FILE (" << filename
                  << ") has event Moved from (" << oldFilename << ")\n";
        for (const auto& callback : mMoveFileCallbacks)
        {
          callback(dir, filename, oldFilename);
        }
        break;
      default: std::cout << "Should never happen!\n";
    }
  }
}