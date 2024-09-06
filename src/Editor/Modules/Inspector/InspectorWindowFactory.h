#pragma once

#include "Editor/Modules/IGuiModule.h"
#include "Editor/Modules/Inspector/IInspectorWindowFactory.h"

namespace Dwarf
{
  class InspectorWindowFactory : public IInspectorWindowFactory
  {
  private:
    std::function<boost::di::injector<InspectorWindow>()> m_InjectorFactory;

  public:
    BOOST_DI_INJECT(InspectorWindowFactory,
                    std::shared_ptr<IEditorSelection> selection,
                    std::shared_ptr<IAssetDatabase>   assetDatabase,
                    std::shared_ptr<IAssetInspector>  assetInspector,
                    std::shared_ptr<IEntityInspector> entityInspector);
    ~InspectorWindowFactory() override = default;
    std::shared_ptr<InspectorWindow>
    Create() const override;

    std::shared_ptr<InspectorWindow>
    Create(SerializedModule serializedModule) const override;
  };
}