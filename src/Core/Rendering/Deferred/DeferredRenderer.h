#pragma once

#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/RendererApi.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Camera.h"

namespace Dwarf
{
    class DeferredRenderer : public Renderer
    {
    private:
        Ref<RendererApi> rendererApi;

    public:
        DeferredRenderer();
        ~DeferredRenderer();
        virtual void RenderScene(Ref<Scene> scene, Ref<Camera> camera, glm::ivec2 viewportSize, boolean renderGrid) override;
        virtual Ref<Framebuffer> CreateFramebuffer(glm::ivec2 resolution) override;
    };
}