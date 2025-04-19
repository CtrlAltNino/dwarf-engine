#pragma once

#include "Editor/Modules/SceneViewer/SceneViewerWindow.hpp"

namespace Dwarf
{
  /**
   * @brief Factory class for creating SceneViewerWindow instances
   *
   */
  class ISceneViewerWindowFactory
  {
  public:
    virtual ~ISceneViewerWindowFactory() = default;

    /**
     * @brief Creates a default SceneViewerWindow instance
     *
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<SceneViewerWindow> = 0;

    /**
     * @brief Creates a SceneViewerWindow instance based off serialized data
     *
     * @param serializedModule Serialized data of a SceneViewerWindow
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create(SerializedModule serializedModule) const
      -> std::unique_ptr<SceneViewerWindow> = 0;
  };
}