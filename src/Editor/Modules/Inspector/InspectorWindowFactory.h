#pragma once

#include "Editor/Modules/IGuiModule.h"
#include "Editor/Modules/Inspector/IInspectorWindowFactory.h"
#include "Logging/IDwarfLogger.h"
#include <boost/di.hpp>
#include <memory>

namespace Dwarf
{
  class InspectorWindowFactory : public IInspectorWindowFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>     mLogger;
    std::shared_ptr<IEditorSelection> mSelection;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    std::shared_ptr<IAssetInspector>  mAssetInspector;
    std::shared_ptr<IEntityInspector> mEntityInspector;

  public:
    InspectorWindowFactory(std::shared_ptr<IDwarfLogger>     logger,
                           std::shared_ptr<IEditorSelection> selection,
                           std::shared_ptr<IAssetDatabase>   assetDatabase,
                           std::shared_ptr<IAssetInspector>  assetInspector,
                           std::shared_ptr<IEntityInspector> entityInspector);
    ~InspectorWindowFactory() override;

    /**
     * @brief Creates a default InspectorWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<InspectorWindow> override;

    /**
     * @brief Creates a InspectorWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a InspectorWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<InspectorWindow> override;
  };
}