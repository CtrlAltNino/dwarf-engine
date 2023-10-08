#pragma once

#include <glm/vec4.hpp>

#include "Core/Rendering/RendererApi.h"
#include "Platform/OpenGL/OpenGLMesh.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Core/Rendering/Material.h"

namespace Dwarf
{
    class OpenGLRendererApi : public RendererApi
    {
    public:
        OpenGLRendererApi();
        ~OpenGLRendererApi();

        virtual void Init() override;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        virtual void SetClearColor(const glm::vec4 &color) override;
        virtual void Clear() override;

        virtual void RenderIndexed(Ref<Mesh> mesh, Ref<Material> material, glm::mat4 modelMatrix, glm::mat4 viewMatrix, glm::mat4 projectionMatrix) override;
    };
}