#pragma once

#include "Core/Base.hpp"
#include "IGraphicsContextFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class GraphicsContextFactory : public IGraphicsContextFactory
  {
  private:
    GraphicsApi                   mApi;
    std::shared_ptr<IDwarfLogger> mLogger;

  public:
    GraphicsContextFactory(GraphicsApi                   api,
                           std::shared_ptr<IDwarfLogger> logger);
    ~GraphicsContextFactory() override;

    /**
     * @brief Creates a graphics context and connects it to the SDL window
     *
     * @param window The SDL window to attach the graphics context to
     * @return Unique pointer to the creates graphics context
     */
    auto
    Create(SDL_Window* window) const
      -> std::unique_ptr<IGraphicsContext> override;
  };
} // namespace Dwarf