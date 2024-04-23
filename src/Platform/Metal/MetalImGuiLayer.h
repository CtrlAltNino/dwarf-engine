#pragma once

#include "Core/UI/ImGuiLayer.h"

namespace Dwarf
{
  class MetalImGuiLayer : public ImGuiLayer
  {
  private:
    MTLRenderPassDescriptor* m_RenderPassDescriptor;

  public:
    MetalImGuiLayer();
    ~MetalImGuiLayer();

    virtual void
    OnAttach(SDL_Window* window) override;
    virtual void
    OnDetach() override;

    virtual void
    Begin() override;
    virtual void
    End() override;

    virtual void
    HandleSDLEvent(SDL_Event* event) override;
  };
}