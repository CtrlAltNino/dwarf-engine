#pragma once

#include "Core/Base.h"
#include "IGraphicsContextFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class GraphicsContextFactory : public IGraphicsContextFactory
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    GraphicsApi                   m_Api;

  public:
    GraphicsContextFactory(std::shared_ptr<IDwarfLogger> logger,
                           GraphicsApi                   api);
    ~GraphicsContextFactory() override;
    virtual std::shared_ptr<IGraphicsContext>
    Create(SDL_Window* window) const override;
  };
} // namespace Dwarf