#pragma once

#include "pch.h"

#include "Core/Base.h"
#include "Core/Rendering/Mesh/IMesh.h"
#include "Core/Rendering/Material/Material.h"
#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Shader/IComputeShader.h"

namespace Dwarf
{
  class RendererApi
  {
  public:
    virtual ~RendererApi() = default;
    virtual void
    Init() = 0;
    virtual void
    SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void
    SetClearColor(const glm::vec4& color) = 0;
    virtual void
    Clear() = 0;
    virtual void
    Clear(unsigned int value) = 0;
    virtual void
    RenderIndexed(std::shared_ptr<IMesh>     mesh,
                  std::shared_ptr<IMaterial> material,
                  glm::mat4                  modelMatrix,
                  glm::mat4                  viewMatrix,
                  glm::mat4                  projectionMatrix) = 0;
    virtual void
    ApplyComputeShader(std::shared_ptr<IComputeShader> computeShader,
                       std::shared_ptr<Framebuffer>    fb,
                       uint32_t                        sourceAttachment,
                       uint32_t destinationAttachment) = 0;
    virtual void
    Blit(std::shared_ptr<Framebuffer> source,
         std::shared_ptr<Framebuffer> destination,
         uint32_t                     sourceAttachment,
         uint32_t                     destinationAttachment,
         uint32_t                     width,
         uint32_t                     height) = 0;
  };
}