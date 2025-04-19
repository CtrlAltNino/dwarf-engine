#pragma once

#include "Editor/Modules/SceneHierarchy/SceneHierarchyWindow.hpp"

namespace Dwarf
{
  /**
   * @brief Factory class that creates SceneHierarchyWindow instances
   *
   */
  class ISceneHierarchyWindowFactory
  {
  public:
    virtual ~ISceneHierarchyWindowFactory() = default;

    /**
     * @brief Creates a default SceneHierarchyWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<SceneHierarchyWindow> = 0;

    /**
     * @brief Creates a SceneHierarchyWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a SceneHierarchyWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<SceneHierarchyWindow> = 0;
  };
} // namespace Dwarf