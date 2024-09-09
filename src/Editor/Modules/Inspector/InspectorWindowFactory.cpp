#include "InspectorWindowFactory.h"

namespace Dwarf
{
  InspectorWindowFactory::InspectorWindowFactory(
    std::shared_ptr<IEditorSelection> selection,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetInspector>  assetInspector,
    std::shared_ptr<IEntityInspector> entityInspector)
    : m_Selection(selection)
    , m_AssetDatabase(assetDatabase)
    , m_AssetInspector(assetInspector)
    , m_EntityInspector(entityInspector)
  {
  }

  std::unique_ptr<InspectorWindow>
  InspectorWindowFactory::Create() const
  {
    return std::make_unique<InspectorWindow>(
      m_Selection, m_AssetDatabase, m_AssetInspector, m_EntityInspector);
  }

  std::unique_ptr<InspectorWindow>
  InspectorWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<InspectorWindow>(m_Selection,
                                             m_AssetDatabase,
                                             m_AssetInspector,
                                             m_EntityInspector,
                                             serializedModule);
  }
} // namespace Dwarf