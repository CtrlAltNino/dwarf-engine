#include "pch.h"

#include "AssetReimporter.h"

namespace Dwarf
{
  AssetReimporter::AssetReimporter(
    std::shared_ptr<IDwarfLogger>                               logger,
    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> assetDatabase)
    : mLogger(std::move(logger))
    , mAssetDatabase(assetDatabase)
  {
    mLogger->LogDebug(Log("AssetReimporter created", "AssetReimporter"));
  }

  void
  AssetReimporter::QueueReimport(const std::filesystem::path& assetPath)
  {
    mLogger->LogDebug(
      Log(fmt::format("Queued reimport for asset: {}", assetPath.string()),
          "AssetReimporter"));
    {
      std::unique_lock<std::mutex> lock(mReimportMutex);
      mReimportQueue.push_back(assetPath);
    }
  }

  void
  AssetReimporter::ReimportQueuedAssets()
  {
    {
      std::unique_lock<std::mutex> lock(mReimportMutex);

      for (const auto& assetPath : mReimportQueue)
      {
        mLogger->LogDebug(
          Log(fmt::format("Reimporting asset: {}", assetPath.string()),
              "AssetReimporter"));
        if (mAssetDatabase.get()->Exists(assetPath))
        {
          mAssetDatabase.get()->Reimport(assetPath);
        }
        else
        {
          mAssetDatabase.get()->Import(assetPath);
        }
      }

      mReimportQueue.clear();
    }
  }
}