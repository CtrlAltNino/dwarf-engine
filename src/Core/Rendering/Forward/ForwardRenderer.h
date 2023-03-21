#pragma once

#include "Core/Rendering/Forward/RenderCommand.h"

class ForwardRenderer {
    public:
        void Submit(RenderCommand command);
        void Render();
};