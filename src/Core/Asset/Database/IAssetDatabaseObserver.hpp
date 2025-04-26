#pragma once

namespace Dwarf
{
  class IAssetDatabaseObserver
  {
  public:
    virtual ~IAssetDatabaseObserver() = default;

    virtual void
    OnReimportAll() = 0;
  };
}