#include "InspectorWindowFactory.h"

namespace Dwarf
{
  InspectorWindowFactory::InspectorWindowFactory(
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<IEditorSelection> selection,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetInspector>  assetInspector,
    std::shared_ptr<IEntityInspector> entityInspector)
    : mLogger(std::move(logger))
    , mSelection(std::move(selection))
    , mAssetDatabase(std::move(assetDatabase))
    , mAssetInspector(std::move(assetInspector))
    , mEntityInspector(std::move(entityInspector))
  {
    mLogger->LogDebug(
      Log("InspectorWindowFactory created", "InspectorWindowFactory"));
  }
  InspectorWindowFactory::~InspectorWindowFactory()
  {
    mLogger->LogDebug(
      Log("InspectorWindowFactory destroyed", "InspectorWindowFactory"));
  }

  auto
  InspectorWindowFactory::Create() const -> std::unique_ptr<InspectorWindow>
  {
    return std::make_unique<InspectorWindow>(
      mLogger, mSelection, mAssetDatabase, mAssetInspector, mEntityInspector);
  }

  auto
  InspectorWindowFactory::Create(SerializedModule serializedModule) const
    -> std::unique_ptr<InspectorWindow>
  {
    return std::make_unique<InspectorWindow>(mLogger,
                                             mSelection,
                                             mAssetDatabase,
                                             mAssetInspector,
                                             mEntityInspector,
                                             serializedModule);
  }
} // namespace Dwarf