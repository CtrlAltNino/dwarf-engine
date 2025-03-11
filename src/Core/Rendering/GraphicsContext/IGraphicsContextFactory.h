#pragma once

#include "SDL3/SDL.h"

#include "IGraphicsContext.h"

namespace Dwarf
{
  class IGraphicsContextFactory
  {
  public:
    virtual ~IGraphicsContextFactory() = default;

    virtual std::unique_ptr<IGraphicsContext>
    Create(SDL_Window* window) const = 0;
  };
} // namespace Dwarf