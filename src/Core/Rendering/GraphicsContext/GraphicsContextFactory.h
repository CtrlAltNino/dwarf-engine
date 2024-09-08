#pragma once

#include "Core/Base.h"
#include "IGraphicsContextFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class GraphicsContextFactory : public IGraphicsContextFactory
  {
  private:
    IDwarfLogger& m_Logger;
    GraphicsApi   m_Api;

  public:
    GraphicsContextFactory(IDwarfLogger& logger, GraphicsApi api);
    ~GraphicsContextFactory() override;

    virtual std::unique_ptr<IGraphicsContext>
    Create(SDL_Window* window) const override;
  };
} // namespace Dwarf