#include "pch.h"

#include "DebugWindowFactory.h"

namespace Dwarf
{
  DebugWindowFactory::DebugWindowFactory(
    std::shared_ptr<IDwarfLogger>   logger,
    std::shared_ptr<IAssetDatabase> assetDatabase)
    : mLogger(std::move(logger))
    , mAssetDatabase(std::move(assetDatabase))

  {
    mLogger->LogDebug(Log("DebugWindowFactory created", "DebugWindowFactory"));
  }

  DebugWindowFactory::~DebugWindowFactory()
  {
    mLogger->LogDebug(
      Log("DebugWindowFactory destroyed", "DebugWindowFactory"));
  }

  auto
  DebugWindowFactory::Create() const -> std::unique_ptr<DebugWindow>
  {
    return std::make_unique<DebugWindow>(mLogger, mAssetDatabase);
  }

  auto
  DebugWindowFactory::Create(SerializedModule serializedModule) const
    -> std::unique_ptr<DebugWindow>
  {
    return std::make_unique<DebugWindow>(
      mLogger, mAssetDatabase, serializedModule);
  }
}