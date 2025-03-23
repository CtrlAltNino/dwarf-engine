#pragma once

#include "Editor/Modules/IGuiModule.h"
#include "Editor/Modules/Performance/PerformanceWindow.h"

namespace Dwarf
{
  /**
   * @brief Factory class that creates PerformanceWindow instances
   *
   */
  class IPerformanceWindowFactory
  {
  public:
    virtual ~IPerformanceWindowFactory() = default;

    /**
     * @brief Creates a default PerformanceWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<PerformanceWindow> = 0;

    /**
     * @brief Creates a PerformanceWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a PerformanceWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<PerformanceWindow> = 0;
  };
} // namespace Dwarf