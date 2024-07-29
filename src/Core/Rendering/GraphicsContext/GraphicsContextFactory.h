#pragma once

#include "Core/Base.h"
#include "IGraphicsContextFactory.h"

namespace Dwarf
{
  class GraphicsContextFactory : public IGraphicsContextFactory
  {
  public:
    GraphicsContextFactory(GraphicsApi api);
    virtual std::unique_ptr<IGraphicsContext>
    Create(SDL_Window* window) override;

  private:
    GraphicsApi m_Api;
  };
} // namespace Dwarf