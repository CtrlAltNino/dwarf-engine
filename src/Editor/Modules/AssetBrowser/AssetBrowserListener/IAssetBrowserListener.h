#pragma once

#include <efsw/efsw.hpp>

namespace Dwarf
{
  /// @brief EFSW listener class
  class IAssetBrowserListener : public efsw::FileWatchListener
  {
  public:
    virtual ~IAssetBrowserListener() = default;
  };
}