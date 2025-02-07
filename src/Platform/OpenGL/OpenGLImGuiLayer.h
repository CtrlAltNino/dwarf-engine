#pragma once

#include "Logging/IDwarfLogger.h"
#include "UI/IImGuiLayer.h"

namespace Dwarf
{
  class OpenGLImGuiLayer : public IImGuiLayer
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;

  public:
    OpenGLImGuiLayer(std::shared_ptr<IDwarfLogger> logger);
    ~OpenGLImGuiLayer() override;

    void
    OnAttach(SDL_Window* window) override;
    void
    OnDetach() override;

    void
    Begin() override;
    void
    End() override;

    void
    HandleSDLEvent(SDL_Event* event) override;
  };
}