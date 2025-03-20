#pragma once
#include "Core/UUID.h"
#include "Utilities/ISerializable.h"
#include "pch.h"
#include <boost/serialization/strong_typedef.hpp>
#include <imgui.h>
#include <imgui_internal.h>

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
    std::string mLabel;

    /// @brief Enum identification.
    MODULE_TYPE mModuleType;

    /// @brief Incremented global GUI module ID.
    std::shared_ptr<UUID> mId;

    /// @brief Flag to check if the window is collapsed or not.
    bool mWindowOpened = true;

  public:
    IGuiModule(ModuleLabel name, ModuleType type, ModuleID id)
      : mLabel(name)
      , mModuleType(type)
      , mId(id)
    {
    }

    virtual ~IGuiModule() = default;

    /// @brief Returns the name of the module.
    /// @return Name of the module.
    std::string
    GetModuleName() const
    {
      return mLabel;
    }

    /// @brief Returns thhe type of the module.
    /// @return Type of the module.
    MODULE_TYPE
    GetModuleType() const { return mModuleType; }

    /// @brief Returns the global module index.
    /// @return The module index.
    std::shared_ptr<UUID>
    GetUuid() const
    {
      return mId;
    }

    /// @brief Generates the ImGui window identifier for a module window..
    /// @return The window identifier.
    std::string
    GetIdentifier() const
    {
      return std::format("{}##{}", mLabel, mId->toString());
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
      return !mWindowOpened;
    }

    virtual void
    OnUpdate() = 0;

    /// @brief Renders the module window.
    virtual void
    OnImGuiRender() = 0;
  };
}