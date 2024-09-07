#pragma once

#include "UI/IImGuiLayer.h"

namespace Dwarf
{
  class OpenGLImGuiLayer : public IImGuiLayer
  {
  public:
    OpenGLImGuiLayer() = default;
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