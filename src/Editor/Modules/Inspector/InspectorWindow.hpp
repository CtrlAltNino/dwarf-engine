#pragma once

#include "AssetInspector/IAssetInspector.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Editor/Modules/IGuiModule.hpp"
#include "Editor/Selection/IEditorSelection.hpp"
#include "EntityInspector/IEntityInspector.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  /// @brief Module that renders a window, containing information of selected
  /// objects or assets.
  class InspectorWindow : public IGuiModule
  {
  private:
    /// @brief Pointer to the currently opened scene instance.
    std::shared_ptr<IDwarfLogger>     mLogger;
    std::shared_ptr<IEditorSelection> mSelection;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    std::shared_ptr<IAssetInspector>  mAssetInspector;
    std::shared_ptr<IEntityInspector> mEntityInspector;

  public:
    InspectorWindow(std::shared_ptr<IDwarfLogger>     logger,
                    std::shared_ptr<IEditorSelection> selection,
                    std::shared_ptr<IAssetDatabase>   assetDatabase,
                    std::shared_ptr<IAssetInspector>  assetInspector,
                    std::shared_ptr<IEntityInspector> entityInspector);

    InspectorWindow(std::shared_ptr<IDwarfLogger>     logger,
                    std::shared_ptr<IEditorSelection> selection,
                    std::shared_ptr<IAssetDatabase>   assetDatabase,
                    std::shared_ptr<IAssetInspector>  assetInspector,
                    std::shared_ptr<IEntityInspector> entityInspector,
                    SerializedModule                  serializedModule);

    ~InspectorWindow() override;

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    void
    OnUpdate() override;

    auto
    Serialize() -> nlohmann::json override;
  };
}