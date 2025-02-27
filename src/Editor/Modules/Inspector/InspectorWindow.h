#pragma once
#include "pch.h"
#include "Editor/Modules/IGuiModule.h"
#include "AssetInspector/IAssetInspector.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/IEditor.h"
#include "Editor/Selection/IEditorSelection.h"
#include "EntityInspector/IEntityInspector.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief Module that renders a window, containing information of selected
  /// objects or assets.
  class InspectorWindow : public IGuiModule
  {
  private:
    /// @brief Pointer to the currently opened scene instance.
    std::shared_ptr<IEditorSelection> m_Selection;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IAssetInspector>  m_AssetInspector;
    std::shared_ptr<IEntityInspector> m_EntityInspector;

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