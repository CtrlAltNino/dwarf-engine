#pragma once

#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/RendererApi.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Camera.h"

namespace Dwarf{
    class ForwardRenderer : public Renderer {
        private:
            Ref<RendererApi> m_RendererApi;
        public:
            ForwardRenderer();
            ~ForwardRenderer();
            virtual void RenderScene(Ref<Scene> scene, Ref<Camera> camera, glm::ivec2 viewportSize) override;
            virtual Ref<Framebuffer> CreateFramebuffer(glm::ivec2 resolution) override;
    };
}