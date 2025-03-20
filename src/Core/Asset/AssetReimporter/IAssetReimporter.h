#pragma once
#include <filesystem>

namespace Dwarf
{
  /**
   * @brief A class that manages reimport requests for a multi threaded
   * approach
   *
   */
  class IAssetReimporter
  {
  public:
    virtual ~IAssetReimporter() = default;

    /**
     * @brief Enqueues a reimport request
     *
     * @param assetPath The path of the asset that should be reimported
     */
    virtual void
    QueueReimport(const std::filesystem::path& assetPath) = 0;

    /**
     * @brief Processes the current reimport requests
     *
     */
    virtual void
    ReimportQueuedAssets() = 0;
  };
}