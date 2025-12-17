#pragma once

namespace Dwarf
{
  /**
   * @brief Interface class representing the renderer for skyboxes
   *
   */
  class ISkyboxRenderer
  {
  public:
    virtual ~ISkyboxRenderer() = default;

    /**
     * @brief Sets the view matrix to use for the skybox rendering
     *
     * @param viewMat
     */
    virtual void
    SetViewMatrix(glm::mat4 viewMat) = 0;

    /**
     * @brief Sets the projection matrix to use for the skybox rendering
     *
     * @param projectionMat
     */
    virtual void
    SetProjectionMatrix(glm::mat4 projectionMat) = 0;

    /**
     * @brief Renders the skybox
     *
     */
    virtual void
    Render() = 0;
  };
}