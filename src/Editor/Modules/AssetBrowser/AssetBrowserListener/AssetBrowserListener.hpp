#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "IAssetBrowserListener.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <efsw/efsw.hpp>

namespace Dwarf
{
  /// @brief EFSW listener class
  class AssetBrowserListener
    : public efsw::FileWatchListener
    , public IAssetBrowserListener
  {
  private:
    std::function<void()> mCallback;

    /// @brief The EFSW file watcher isntance
    efsw::FileWatcher mFileWatcher;

    /// @brief Watch ID for the EFSW file watcher.
    efsw::WatchID mWatchID;

    std::shared_ptr<IDwarfLogger> mLogger;

  public:
    AssetBrowserListener(const AssetDirectoryPath&     assetDirectoryPath,
                         std::shared_ptr<IDwarfLogger> logger,
                         std::function<void()>         callback);
    ~AssetBrowserListener() override;

    void
    handleFileAction(efsw::WatchID      watchid,
                     const std::string& dir,
                     const std::string& filename,
                     efsw::Action       action,
                     std::string        oldFilename) override;
  };
}