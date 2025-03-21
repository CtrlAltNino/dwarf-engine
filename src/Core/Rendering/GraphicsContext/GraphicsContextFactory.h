#pragma once

#include "Core/Base.h"
#include "IGraphicsContextFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class GraphicsContextFactory : public IGraphicsContextFactory
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    GraphicsApi                   mApi;

  public:
    GraphicsContextFactory(std::shared_ptr<IDwarfLogger> logger,
                           GraphicsApi                   api);
    ~GraphicsContextFactory() override;

    auto
    Create(SDL_Window* window) const
      -> std::unique_ptr<IGraphicsContext> override;
  };
} // namespace Dwarf