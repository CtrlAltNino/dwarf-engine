#include "AssetReimporter.h"

#include <fmt/format.h>

namespace Dwarf
{
  AssetReimporter::AssetReimporter(
    IDwarfLogger&                                               logger,
    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> assetDatabase)
    : m_Logger(logger)
    , m_AssetDatabase(assetDatabase)
  {
    m_Logger.LogDebug(Log("AssetReimporter created", "AssetReimporter"));
  }

  void
  AssetReimporter::QueueReimport(const std::filesystem::path& assetPath)
  {
    m_Logger.LogDebug(
      Log(fmt::format("Queued reimport for asset: {}", assetPath.string()),
          "AssetReimporter"));
    m_ReimportQueue.push_back(assetPath);
  }

  void
  AssetReimporter::ReimportQueuedAssets()
  {
    for (const auto& assetPath : m_ReimportQueue)
    {
      m_Logger.LogDebug(
        Log(fmt::format("Reimporting asset: {}", assetPath.string()),
            "AssetReimporter"));
      if (m_AssetDatabase.get()->Exists(assetPath))
      {
        m_AssetDatabase.get()->Reimport(assetPath);
      }
      else
      {
        m_AssetDatabase.get()->Import(assetPath);
      }
    }

    m_ReimportQueue.clear();
  }
}