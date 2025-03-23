#pragma once

namespace Dwarf
{
  class IAssetInspector
  {
  public:
    virtual ~IAssetInspector() = default;
    virtual void
    Render() = 0;
  };
}