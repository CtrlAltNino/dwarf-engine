#pragma once

#include "Core/Rendering/RendererApi/IRendererApi.hpp"

namespace Dwarf
{
  class VulkanRendererApi : public IRendererApi
  {
  public:
    VulkanRendererApi();
    ~VulkanRendererApi();
  };
}