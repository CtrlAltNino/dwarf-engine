#include "InspectorWindowFactory.h"

namespace Dwarf
{
  InspectorWindowFactory::InspectorWindowFactory(
    std::shared_ptr<IEditorSelection> selection,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetInspector>  assetInspector,
    std::shared_ptr<IEntityInspector> entityInspector)
    : m_InjectorFactory(
        [&selection, &assetDatabase, &assetInspector, &entityInspector]()
        {
          return boost::di::make_injector(
            boost::di::bind<IEditorSelection>.to(selection),
            boost::di::bind<IAssetDatabase>.to(assetDatabase),
            boost::di::bind<IAssetInspector>.to(assetInspector),
            boost::di::bind<IEntityInspector>.to(entityInspector));
        })
  {
  }

  std::shared_ptr<InspectorWindow>
  InspectorWindowFactory::Create() const
  {
    return std::make_shared<InspectorWindow>(
      m_InjectorFactory().create<InspectorWindow>());
  }

  std::shared_ptr<InspectorWindow>
  InspectorWindowFactory::Create(SerializedModule serializedModule) const
  {
    auto injector = boost::di::make_injector(
      m_InjectorFactory(),
      boost::di::bind<SerializedModule>.to(serializedModule));

    return std::make_shared<InspectorWindow>(
      injector.create<InspectorWindow>());
  }
} // namespace Dwarf