#include "dpch.h"

#include "Core/Rendering/Deferred/DeferredRenderer.h"

namespace Dwarf {
    DeferredRenderer::DeferredRenderer(){}
    DeferredRenderer::~DeferredRenderer(){}

    void DeferredRenderer::RenderScene(Ref<Scene> scene, Ref<Camera> camera){

    }

    Ref<Framebuffer> DeferredRenderer::CreateFramebuffer(glm::ivec2 resolution){
        return nullptr;
    }
}