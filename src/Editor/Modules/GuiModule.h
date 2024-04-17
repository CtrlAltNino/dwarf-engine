#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <nlohmann/json.hpp>
#include <format>

#include "Editor/EditorModel.h"

namespace Dwarf
{

  /// @brief Enum defining the types of GUI modules.
  enum class MODULE_TYPE
  {
    PERFORMANCE,
    SCENE_GRAPH,
    CONSOLE,
    SCENE_VIEWER,
    ASSET_BROWSER,
    INSPECTOR,
    DEBUG
  };

  /// @brief GUI module base class.
  class GuiModule
  {
  protected:
    /// @brief Interfaced editor controller to communicate with the controller.
    Ref<EditorModel> m_Model;

    /// @brief Name to use for the module.
    std::string m_Label;

    /// @brief Enum identification.
    MODULE_TYPE m_ModuleType;

    /// @brief Incremented global GUI module ID.
    int m_Index;

    /// @brief Flag to check if the window is collapsed or not.
    bool m_WindowOpened = true;

  public:
    GuiModule(Ref<EditorModel> model,
              std::string_view name,
              MODULE_TYPE      type,
              int              index)
      : m_Model(model)
      , m_Label(name)
      , m_ModuleType(type)
      , m_Index(index)
    {
    }

    virtual ~GuiModule() = default;

    /// @brief Returns the name of the module.
    /// @return Name of the module.
    std::string
    GetModuleName() const
    {
      return m_Label;
    }

    /// @brief Returns thhe type of the module.
    /// @return Type of the module.
    MODULE_TYPE
    GetModuleType() const { return m_ModuleType; }

    /// @brief Returns the global module index.
    /// @return The module index.
    int
    GetIndex() const
    {
      return m_Index;
    }

    /// @brief Generates the ImGui window identifier for a module window..
    /// @return The window identifier.
    std::string
    GetIdentifier() const
    {
      return std::format("{}##{}", m_Label, std::to_string(m_Index));
    }

    /// @brief Generates a ImGui ID for the module.
    /// @return The ImGui ID.
    ImGuiID
    GetImGuiID()
    {
      return ImGui::GetID(GetIdentifier().c_str());
    }

    bool
    GetWindowClose()
    {
      return !m_WindowOpened;
    }

    virtual void
    OnUpdate(double deltaTime) = 0;

    /// @brief Renders the module window.
    virtual void
    OnImGuiRender() = 0;

    virtual std::string
    Serialize() = 0;

    virtual void
    Deserialize(nlohmann::json moduleData) = 0;
  };
}