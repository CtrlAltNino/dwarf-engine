#pragma once

#include "Core/Rendering/RendererApi/IRendererApi.h"

namespace Dwarf
{
  class VulkanRendererApi : public IRendererApi
  {
  public:
    VulkanRendererApi();
    ~VulkanRendererApi();
  };
}