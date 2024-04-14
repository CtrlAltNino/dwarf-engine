#pragma once

#include "Core/UI/ImGuiLayer.h"

namespace Dwarf {
  class OpenGLImGuiLayer : public ImGuiLayer
  {
  public:
    OpenGLImGuiLayer() = default;
    ~OpenGLImGuiLayer() override = default;

    void OnAttach(SDL_Window* window) override;
    void OnDetach() override;

    void Begin() override;
    void End() override;

    void HandleSDLEvent(SDL_Event* event) override;
  };
}