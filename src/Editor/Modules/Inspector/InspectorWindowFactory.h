#pragma once

#include "Editor/Modules/IGuiModule.h"
#include "Editor/Modules/Inspector/IInspectorWindowFactory.h"
#include <memory>
#include <boost/di.hpp>

namespace Dwarf
{
  class InspectorWindowFactory : public IInspectorWindowFactory
  {
  private:
    std::shared_ptr<IEditorSelection> m_Selection;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IAssetInspector>  m_AssetInspector;
    std::shared_ptr<IEntityInspector> m_EntityInspector;

  public:
    BOOST_DI_INJECT(InspectorWindowFactory,
                    std::shared_ptr<IEditorSelection> selection,
                    std::shared_ptr<IAssetDatabase>   assetDatabase,
                    std::shared_ptr<IAssetInspector>  assetInspector,
                    std::shared_ptr<IEntityInspector> entityInspector);
    ~InspectorWindowFactory() override = default;
    std::unique_ptr<InspectorWindow>
    Create() const override;

    std::unique_ptr<InspectorWindow>
    Create(SerializedModule serializedModule) const override;
  };
}