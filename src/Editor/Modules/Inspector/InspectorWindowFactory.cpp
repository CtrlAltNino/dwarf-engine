#include "InspectorWindowFactory.h"

namespace Dwarf
{
  InspectorWindowFactory::InspectorWindowFactory(
    std::shared_ptr<IEditorSelection> selection,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetInspector>  assetInspector,
    std::shared_ptr<IEntityInspector> entityInspector)
    : mSelection(selection)
    , mAssetDatabase(assetDatabase)
    , mAssetInspector(assetInspector)
    , mEntityInspector(entityInspector)
  {
  }

  std::unique_ptr<InspectorWindow>
  InspectorWindowFactory::Create() const
  {
    return std::make_unique<InspectorWindow>(
      mSelection, mAssetDatabase, mAssetInspector, mEntityInspector);
  }

  std::unique_ptr<InspectorWindow>
  InspectorWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<InspectorWindow>(mSelection,
                                             mAssetDatabase,
                                             mAssetInspector,
                                             mEntityInspector,
                                             serializedModule);
  }
} // namespace Dwarf