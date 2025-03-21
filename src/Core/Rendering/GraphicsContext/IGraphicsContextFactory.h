#pragma once

#include "SDL3/SDL.h"

#include "IGraphicsContext.h"

namespace Dwarf
{
  class IGraphicsContextFactory
  {
  public:
    virtual ~IGraphicsContextFactory() = default;

    virtual auto
    Create(SDL_Window* window) const -> std::unique_ptr<IGraphicsContext> = 0;
  };
} // namespace Dwarf