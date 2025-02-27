#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Material/IMaterial.h"
#include "Core/Rendering/Mesh/IMesh.h"
#include "Core/Rendering/Shader/IComputeShader.h"
#include "Core/Scene/Camera/ICamera.h"
namespace Dwarf
{
  struct VRAMUsageBuffer
  {
    int totalMemoryMb;
    int usedMemoryMb;
  };
  class IRendererApi
  {
  public:
    virtual ~IRendererApi() = default;
    virtual void
    SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void
    SetClearColor(const glm::vec4& color) = 0;
    virtual void
    Clear() = 0;
    virtual void
    Clear(unsigned int value) = 0;
    virtual void
    RenderIndexed(IMesh&     mesh,
                  IMaterial& material,
                  ICamera&   camera,
                  glm::mat4  modelMatrix) = 0;
    virtual void
    ApplyComputeShader(std::shared_ptr<IComputeShader> computeShader,
                       std::shared_ptr<IFramebuffer>   fb,
                       uint32_t                        sourceAttachment,
                       uint32_t destinationAttachment) = 0;
    virtual void
    Blit(IFramebuffer& source,
         IFramebuffer& destination,
         uint32_t      sourceAttachment,
         uint32_t      destinationAttachment,
         uint32_t      width,
         uint32_t      height) = 0;

    virtual VRAMUsageBuffer
    QueryVRAMUsage() = 0;

    virtual int32_t
    GetMaxSamples() = 0;
  };
}