#pragma once
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/IEditor.h"
#include "Editor/IEditorSelection.h"
#include "pch.h"
#include "Core/Scene/Scene.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/IGuiModule.h"

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

    /// @brief Renders the components of an entity.
    /// @param entity Entity to render in the inspector.
    void
    RenderComponents(Entity entity);

    void
    BeginComponent(const char* componentHeader) const;

    void
    EndComponent() const;

    template<typename T>
    void
    RenderComponent(T& component);

  public:
    InspectorWindow(std::optional<nlohmann::json>     serializedModule,
                    std::shared_ptr<IEditor>          editor,
                    std::shared_ptr<IEditorSelection> selection,
                    std::shared_ptr<IAssetDatabase>   assetDatabase);

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