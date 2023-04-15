#pragma once

#include "Core/Rendering/RendererApi.h"

namespace Dwarf {
    class OpenGLRendererApi : public RendererApi {
        public:
            OpenGLRendererApi();
            ~OpenGLRendererApi();
    };
}