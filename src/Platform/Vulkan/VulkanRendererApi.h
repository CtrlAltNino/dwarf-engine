#pragma once

#include "Core/Rendering/RendererApi.h"

namespace Dwarf {
  class VulkanRendererApi : public RendererApi
  {
  public:
    VulkanRendererApi();
    ~VulkanRendererApi();
  };
}