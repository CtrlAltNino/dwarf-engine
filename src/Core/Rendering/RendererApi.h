#pragma once

#include <glm/vec4.hpp>

#include "Core/Base.h"
#include "Core/Rendering/Mesh.h"
#include "Core/Rendering/Material.h"

namespace Dwarf
{
    class RendererApi
    {
    public:
        static Ref<RendererApi> Create();

        virtual ~RendererApi() = default;
        virtual void Init() = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void SetClearColor(const glm::vec4 &color) = 0;
        virtual void Clear() = 0;
        virtual void RenderIndexed(Ref<Mesh> mesh, Ref<Material> material, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) = 0;
    };
}