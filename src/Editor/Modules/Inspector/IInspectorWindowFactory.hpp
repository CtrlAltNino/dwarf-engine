#pragma once

#include "Editor/Modules/Inspector/InspectorWindow.hpp"

namespace Dwarf
{
  /**
   * @brief Factory class that creates InspectorWindow instances
   *
   */
  class IInspectorWindowFactory
  {
  public:
    virtual ~IInspectorWindowFactory() = default;

    /**
     * @brief Creates a default InspectorWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<InspectorWindow> = 0;

    /**
     * @brief Creates a InspectorWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a InspectorWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<InspectorWindow> = 0;
  };
} // namespace Dwarf