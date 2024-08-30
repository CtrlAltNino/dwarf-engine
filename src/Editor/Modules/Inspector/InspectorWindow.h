#pragma once
#include "AssetInspector/IAssetInspector.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/IEditor.h"
#include "Editor/IEditorSelection.h"
#include "EntityInspector/IEntityInspector.h"
#include "pch.h"
#include "Core/Scene/Scene.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/IGuiModule.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Core/Rendering/PreviewRenderer/ModelPreview/IModelPreview.h"

namespace Dwarf
{
  /// @brief Module that renders a window, containing information of selected
  /// objects or assets.
  class InspectorWindow : public IGuiModule
  {
  private:
    /// @brief Pointer to the currently opened scene instance.
    std::shared_ptr<IEditor>          m_Editor;
    std::shared_ptr<IEditorSelection> m_Selection;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IAssetInspector>  m_AssetInspector;
    std::shared_ptr<IEntityInspector> m_EntityInspector;

  public:
    InspectorWindow(std::optional<nlohmann::json>     serializedModule,
                    std::shared_ptr<IEditor>          editor,
                    std::shared_ptr<IEditorSelection> selection,
                    std::shared_ptr<IAssetDatabase>   assetDatabase,
                    std::shared_ptr<IAssetInspector>  assetInspector,
                    std::shared_ptr<IEntityInspector> entityInspector);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    void
    OnUpdate() override;

    nlohmann::json
    Serialize() const override;

    void
    Deserialize(nlohmann::json moduleData) override;
  };
}