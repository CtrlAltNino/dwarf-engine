#pragma once
#include "pch.h"
#include "Core/Scene/Scene.h"
#include "Editor/EditorModel.h"
#include "Editor/Modules/GuiModule.h"

namespace Dwarf
{

  /// @brief Module that renders a window, containing information of selected
  /// objects or assets.
  class InspectorWindow : public GuiModule
  {
  private:
    /// @brief Pointer to the currently opened scene instance.
    std::shared_ptr<Scene> m_Scene;

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
    InspectorWindow(std::shared_ptr<EditorModel> listener, int id);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    void
    OnUpdate(double deltaTime) override;

    std::string
    Serialize() override;

    void
    Deserialize(nlohmann::json moduleData) override;
  };
}