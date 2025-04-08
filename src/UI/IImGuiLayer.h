#pragma once

#include <SDL3/SDL.h>
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(std::filesystem::path, ImGuiIniFilePath)

  /**
   * @brief Class that represents a single ImGui Context
   *
   */
  class IImGuiLayer
  {
  public:
    virtual ~IImGuiLayer() = default;

    /**
     * @brief Initializes the ImGui context for a given SDL window
     *
     * @param window
     */
    virtual void
    OnAttach(SDL_Window* window) = 0;

    /**
     * @brief Cleans up the imgui context
     *
     */
    virtual void
    OnDetach() = 0;

    /**
     * @brief Start of a frame
     *
     */
    virtual void
    Begin() = 0;

    /**
     * @brief End of a frame
     *
     */
    virtual void
    End() = 0;

    /**
     * @brief Handles a SDL Event
     *
     * @param event
     */
    virtual void
    HandleSDLEvent(SDL_Event* event) = 0;
  };
}