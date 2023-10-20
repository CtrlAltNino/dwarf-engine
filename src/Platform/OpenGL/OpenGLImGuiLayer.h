#pragma once

#include "Core/UI/ImGuiLayer.h"

namespace Dwarf
{
    class OpenGLImGuiLayer : public ImGuiLayer
    {
    public:
        OpenGLImGuiLayer();
        ~OpenGLImGuiLayer();

        virtual void OnAttach(void *window) override;
        virtual void OnDetach() override;

        virtual void Begin() override;
        virtual void End() override;

        virtual void HandleSDLEvent(SDL_Event *event) override;
    };
}