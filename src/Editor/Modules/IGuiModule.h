#pragma once
#include "pch.h"
#include "Utilities/ISerializable.h"
#include <boost/serialization/strong_typedef.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include "Core/UUID.h"

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

  BOOST_STRONG_TYPEDEF(std::string, ModuleLabel);
  BOOST_STRONG_TYPEDEF(MODULE_TYPE, ModuleType);
  BOOST_STRONG_TYPEDEF(std::shared_ptr<UUID>, ModuleID);
  BOOST_STRONG_TYPEDEF(nlohmann::json, SerializedModule);

  /// @brief GUI module base class.
  class IGuiModule : public ISerializable
  {
  protected:
    /// @brief Name to use for the module.
    std::string m_Label;

    /// @brief Enum identification.
    MODULE_TYPE m_ModuleType;

    /// @brief Incremented global GUI module ID.
    std::shared_ptr<UUID> m_Id;

    /// @brief Flag to check if the window is collapsed or not.
    bool m_WindowOpened = true;

  public:
    IGuiModule(ModuleLabel name, ModuleType type, ModuleID id)
      : m_Label(name)
      , m_ModuleType(type)
      , m_Id(id)
    {
    }

    virtual ~IGuiModule() = default;

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
    std::shared_ptr<UUID>
    GetUuid() const
    {
      return m_Id;
    }

    /// @brief Generates the ImGui window identifier for a module window..
    /// @return The window identifier.
    std::string
    GetIdentifier() const
    {
      return std::format("{}##{}", m_Label, m_Id->ToString());
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
    OnUpdate() = 0;

    /// @brief Renders the module window.
    virtual void
    OnImGuiRender() = 0;

    virtual void
    Deserialize(nlohmann::json moduleData) = 0;
  };
}