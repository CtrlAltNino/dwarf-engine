#pragma once
#include "AssetInspector/IAssetInspector.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/IEditor.h"
#include "Editor/Modules/IGuiModule.h"
#include "Editor/Selection/IEditorSelection.h"
#include "EntityInspector/IEntityInspector.h"
#include "pch.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief Module that renders a window, containing information of selected
  /// objects or assets.
  class InspectorWindow : public IGuiModule
  {
  private:
    /// @brief Pointer to the currently opened scene instance.
    std::shared_ptr<IEditorSelection> mSelection;
    std::shared_ptr<IAssetDatabase>   mAssetDatabase;
    std::shared_ptr<IAssetInspector>  mAssetInspector;
    std::shared_ptr<IEntityInspector> mEntityInspector;

  public:
    InspectorWindow(std::shared_ptr<IEditorSelection> selection,
                    std::shared_ptr<IAssetDatabase>   assetDatabase,
                    std::shared_ptr<IAssetInspector>  assetInspector,
                    std::shared_ptr<IEntityInspector> entityInspector);

    InspectorWindow(std::shared_ptr<IEditorSelection> selection,
                    std::shared_ptr<IAssetDatabase>   assetDatabase,
                    std::shared_ptr<IAssetInspector>  assetInspector,
                    std::shared_ptr<IEntityInspector> entityInspector,
                    SerializedModule                  serializedModule);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    void
    OnUpdate() override;

    nlohmann::json
    Serialize() override;
  };
}