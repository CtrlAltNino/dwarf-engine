#pragma once

#include "IGraphicsContext.h"

namespace Dwarf
{
  /**
   * @brief A class that creates Graphics context instances depending on the
   * graphics API used in the project
   *
   */
  class IGraphicsContextFactory
  {
  public:
    virtual ~IGraphicsContextFactory() = default;

    /**
     * @brief Creates a graphics context and connects it to the SDL window
     *
     * @param window The SDL window to attach the graphics context to
     * @return Unique pointer to the creates graphics context
     */
    virtual auto
    Create(SDL_Window* window) const -> std::unique_ptr<IGraphicsContext> = 0;
  };
} // namespace Dwarf