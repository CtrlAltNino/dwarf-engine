#pragma once

#include "Core/Rendering/RendererApi/IRendererApi.h"

namespace Dwarf
{
  class D3D12RendererApi : public IRendererApi
  {
  public:
    D3D12RendererApi();
    ~D3D12RendererApi();
  };
}