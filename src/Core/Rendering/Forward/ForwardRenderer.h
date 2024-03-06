#pragma once

#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/RendererApi.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/Camera.h"

namespace Dwarf
{
    class ForwardRenderer : public Renderer
    {
    private:
        Ref<RendererApi> m_RendererApi;

    public:
        ForwardRenderer();
        virtual ~ForwardRenderer();
        void RenderScene(Ref<Scene> scene, Ref<Camera> camera, glm::ivec2 viewportSize, bool renderGrid) override;
        void RenderModelPreview(Ref<AssetReference<ModelAsset>> modelAsset, Ref<Camera> camera, glm::ivec2 viewportSize, glm::quat rotation) override;
        void RenderMaterialPreview(Ref<AssetReference<MaterialAsset>> materialAsset, Ref<Camera> camera, glm::ivec2 viewportSize, glm::quat rotation) override;
        Ref<Framebuffer> CreateFramebuffer(glm::ivec2 resolution) override;
    };
}