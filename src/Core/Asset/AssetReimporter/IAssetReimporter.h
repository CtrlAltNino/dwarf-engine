#pragma once

namespace Dwarf
{
  class IAssetReimporter
  {
  public:
    virtual ~IAssetReimporter() = default;

    virtual void
    QueueReimport(const std::filesystem::path& assetPath) = 0;

    virtual void
    ReimportQueuedAssets() = 0;
  };
}