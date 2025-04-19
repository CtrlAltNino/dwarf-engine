#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <boost/di/extension/injections/lazy.hpp>
#include <mutex>

namespace Dwarf
{
  class AssetReimporter : public IAssetReimporter
  {
  private:
    std::vector<std::filesystem::path>                          mReimportQueue;
    std::mutex                                                  mReimportMutex;
    std::shared_ptr<IDwarfLogger>                               mLogger;
    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> mAssetDatabase;

  public:
    AssetReimporter(std::shared_ptr<IDwarfLogger> logger,
                    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>>
                      assetDatabase);

    void
    QueueReimport(const std::filesystem::path& assetPath) override;

    void
    ReimportQueuedAssets() override;
  };
}