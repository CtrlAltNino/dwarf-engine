#pragma once

#include "Editor/Modules/IGuiModule.h"
#include "Editor/Modules/Inspector/IInspectorWindowFactory.h"
#include <boost/di.hpp>
#include <memory>

namespace Dwarf
{
  class InspectorWindowFactory : public IInspectorWindowFactory
  {
  private:
    std::shared_ptr<IEditorSelection> mSelection;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    std::shared_ptr<IAssetInspector>  mAssetInspector;
    std::shared_ptr<IEntityInspector> mEntityInspector;

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