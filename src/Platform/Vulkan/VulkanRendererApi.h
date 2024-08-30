#pragma once

#include "Core/Rendering/RendererAPI/IRendererApi.h"

namespace Dwarf
{
  class VulkanRendererApi : public IRendererApi
  {
  public:
    VulkanRendererApi();
    ~VulkanRendererApi();
  };
}