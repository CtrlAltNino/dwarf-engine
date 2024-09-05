#pragma once

#include "SDL2/SDL.h"

#include "IGraphicsContext.h"

namespace Dwarf
{
  class IGraphicsContextFactory
  {
  public:
    virtual ~IGraphicsContextFactory() = default;

    virtual std::shared_ptr<IGraphicsContext>
    Create(SDL_Window* window) const = 0;
  };
} // namespace Dwarf