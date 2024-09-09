#pragma once

#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"

namespace Dwarf
{
  class IAssetBrowserWindowFactory
  {
  public:
    virtual ~IAssetBrowserWindowFactory() = default;
    virtual std::unique_ptr<AssetBrowserWindow>
    Create() const = 0;

    virtual std::unique_ptr<AssetBrowserWindow>
    Create(SerializedModule serializedModule) const = 0;
  };
}