#pragma once

#include "Core/Rendering/RendererApi.h"

namespace Dwarf {
  class D3D12RendererApi : public RendererApi
  {
  public:
    D3D12RendererApi();
    ~D3D12RendererApi();
  };
}