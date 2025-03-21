#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Database/IAssetDirectoryListener.h"
#include "Logging/IDwarfLogger.h"
#include <efsw/efsw.hpp>

namespace Dwarf
{
  /// @brief EFSW listener class
  class AssetDirectoryListener
    : public efsw::FileWatchListener
    , public IAssetDirectoryListener
  {
  private:
    std::vector<
      std::function<void(const std::string& dir, const std::string& filename)>>
      mAddFileCallbacks;
    std::vector<
      std::function<void(const std::string& dir, const std::string& filename)>>
      mDeleteFileCallbacks;
    std::vector<
      std::function<void(const std::string& dir, const std::string& filename)>>
      mModifyFileCallbacks;
    std::vector<std::function<void(const std::string& dir,
                                   const std::string& filename,
                                   std::string        oldFilename)>>
      mMoveFileCallbacks;

    /// @brief The EFSW file watcher isntance
    efsw::FileWatcher mFileWatcher;

    /// @brief Watch ID for the EFSW file watcher.
    efsw::WatchID mWatchID;

    std::shared_ptr<IDwarfLogger> mLogger;

  public:
    AssetDirectoryListener(const AssetDirectoryPath&     assetDirectoryPath,
                           std::shared_ptr<IDwarfLogger> logger);
    ~AssetDirectoryListener() override;

    void
    handleFileAction(efsw::WatchID      watchid,
                     const std::string& dir,
                     const std::string& filename,
                     efsw::Action       action,
                     std::string        oldFilename) override;

    void
    registerAddFileCallback(
      std::function<void(const std::string& dir, const std::string& filename)>
        callback) override;

    void
    registerDeleteFileCallback(
      std::function<void(const std::string& dir, const std::string& filename)>
        callback) override;

    void
    registerModifyFileCallback(
      std::function<void(const std::string& dir, const std::string& filename)>
        callback) override;

    void
    registerMoveFileCallback(
      std::function<void(const std::string& dir,
                         const std::string& filename,
                         std::string        oldFilename)> callback) override;
  };
}