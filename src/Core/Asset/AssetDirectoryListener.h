#pragma once

#include <efsw/efsw.hpp>

namespace Dwarf
{

  /// @brief EFSW listener class
  class AssetDirectoryListener : public efsw::FileWatchListener
  {
  public:
    AssetDirectoryListener();
    ~AssetDirectoryListener() override;
    void
    handleFileAction(efsw::WatchID      watchid,
                     const std::string& dir,
                     const std::string& filename,
                     efsw::Action       action,
                     std::string        oldFilename) override;
  };
}