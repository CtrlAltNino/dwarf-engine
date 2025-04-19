#pragma once

#include "Editor/Modules/DebugInformation/DebugWindow.hpp"
#include "Editor/Modules/IGuiModule.hpp"

namespace Dwarf
{
  /**
   * @brief Factory class that creates DebugWindow instances
   *
   */
  class IDebugWindowFactory
  {
  public:
    virtual ~IDebugWindowFactory() = default;

    /**
     * @brief Creates a default DebugWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<DebugWindow> = 0;

    /**
     * @brief Creates a DebugWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a DebugWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<DebugWindow> = 0;
  };
} // namespace Dwarf