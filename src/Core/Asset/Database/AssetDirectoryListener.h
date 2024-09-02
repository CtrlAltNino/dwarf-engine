#pragma once

#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Database/IAssetDirectoryListener.h"
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
      m_AddFileCallbacks;
    std::vector<
      std::function<void(const std::string& dir, const std::string& filename)>>
      m_DeleteFileCallbacks;
    std::vector<
      std::function<void(const std::string& dir, const std::string& filename)>>
      m_ModifyFileCallbacks;
    std::vector<std::function<void(const std::string& dir,
                                   const std::string& filename,
                                   std::string        oldFilename)>>
      m_MoveFileCallbacks;

    efsw::FileWatcher m_FileWatcher;

    /// @brief Recursive file watcher for the "/Asset" directory.
    // std::shared_ptr<efsw::FileWatchListener> m_AssetDirectoryListener;

    /// @brief Watch ID for the EFSW file watcher.
    efsw::WatchID m_WatchID;

  public:
    AssetDirectoryListener(AssetDirectoryPath assetDirectoryPath);
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