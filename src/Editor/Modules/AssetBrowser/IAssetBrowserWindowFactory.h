#pragma once

#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"

namespace Dwarf
{
  /**
   * @brief Factory class that creates AssetBrowserWindow instances
   *
   */
  class IAssetBrowserWindowFactory
  {
  public:
    virtual ~IAssetBrowserWindowFactory() = default;

    /**
     * @brief Creates a default AssetBrowserWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<AssetBrowserWindow> = 0;

    /**
     * @brief Creates a AssetBrowserWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a AssetBrowserWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<AssetBrowserWindow> = 0;
  };
}