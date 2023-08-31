#include "dpch.h"

#include "Core/Rendering/Forward/ForwardRenderer.h"

namespace Dwarf {
    ForwardRenderer::ForwardRenderer(){
        m_RendererApi = RendererApi::Create();
    }
    ForwardRenderer::~ForwardRenderer(){}

    void ForwardRenderer::RenderScene(Ref<Scene> scene, Ref<Camera> camera){
        
    }

    Ref<Framebuffer> ForwardRenderer::CreateFramebuffer(glm::ivec2 resolution){
        FramebufferSpecification fbSpec;
        fbSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
        fbSpec.Width = resolution.x;
        fbSpec.Height = resolution.y;
        return Framebuffer::Create(fbSpec);
    }
}