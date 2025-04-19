#pragma once

#include "Core/UUID.hpp"
#include "Utilities/ISerializable.hpp"
#include <boost/serialization/strong_typedef.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace Dwarf
{

  /// @brief Enum defining the types of GUI modules.
  enum class MODULE_TYPE : uint8_t
  {
    PERFORMANCE,
    SCENE_GRAPH,
    CONSOLE,
    SCENE_VIEWER,
    ASSET_BROWSER,
    INSPECTOR,
    DEBUG,
    SCENE_SETTINGS
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
    auto
    GetModuleName() const -> std::string
    {
      return mLabel;
    }

    /// @brief Returns thhe type of the module.
    /// @return Type of the module.
    [[nodiscard]] auto
    GetModuleType() const -> MODULE_TYPE
    {
      return mModuleType;
    }

    /// @brief Returns the global module index.
    /// @return The module index.
    [[nodiscard]] auto
    GetUuid() const -> std::shared_ptr<UUID>
    {
      return mId;
    }

    /// @brief Generates the ImGui window identifier for a module window..
    /// @return The window identifier.
    [[nodiscard]] auto
    GetIdentifier() const -> std::string
    {
      return std::format("{}##{}", mLabel, mId->toString());
    }

    /// @brief Generates a ImGui ID for the module.
    /// @return The ImGui ID.
    [[nodiscard]] auto
    GetImGuiID() const -> ImGuiID
    {
      return ImGui::GetID(GetIdentifier().c_str());
    }

    /**
     * @brief Returns if the ImGui window is opened or closed
     *
     * @return true If the window is opened
     * @return false If the window is collapsed
     */
    [[nodiscard]] auto
    GetWindowClose() const -> bool
    {
      return !mWindowOpened;
    }

    /**
     * @brief Runs logic updates of a module (Before rendering)
     *
     */
    virtual void
    OnUpdate() = 0;

    /// @brief Renders the module window.
    virtual void
    OnImGuiRender() = 0;
  };
}