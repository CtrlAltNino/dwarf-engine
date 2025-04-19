#pragma once

#include "IAssetBrowserListener.hpp"

namespace Dwarf
{
  class IAssetBrowserListenerFactory
  {
  public:
    virtual ~IAssetBrowserListenerFactory() = default;

    [[nodiscard]] virtual auto
    Create(std::function<void()> callback) const
      -> std::unique_ptr<IAssetBrowserListener> = 0;
  };
}