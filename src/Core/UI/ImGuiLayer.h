#pragma once

#include "Core/Base.h"
#include <SDL2/SDL.h>

namespace Dwarf {
    class ImGuiLayer {
        public:
            virtual ~ImGuiLayer() = default;

            virtual void OnAttach(void* window) = 0;

            virtual void OnDetach() = 0;

            virtual void Begin() = 0;

            virtual void End() = 0;

            virtual void HandleSDLEvent(SDL_Event* event) = 0;

            static Ref<ImGuiLayer> Create(GraphicsApi api);
    };
}