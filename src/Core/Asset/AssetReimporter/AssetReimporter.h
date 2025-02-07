#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Logging/IDwarfLogger.h"
#include <boost/di/extension/injections/lazy.hpp>

namespace Dwarf
{
  class AssetReimporter : public IAssetReimporter
  {
  private:
    std::vector<std::filesystem::path>                          m_ReimportQueue;
    IDwarfLogger&                                               m_Logger;
    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>> m_AssetDatabase;

  public:
    AssetReimporter(IDwarfLogger& logger,
                    boost::di::extension::lazy<std::shared_ptr<IAssetDatabase>>
                      assetDatabase);

    void
    QueueReimport(const std::filesystem::path& assetPath) override;

    void
    ReimportQueuedAssets() override;
  };
}