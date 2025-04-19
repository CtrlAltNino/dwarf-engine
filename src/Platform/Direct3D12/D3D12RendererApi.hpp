#pragma once

#include "Core/Rendering/RendererApi/IRendererApi.hpp"

namespace Dwarf
{
  class D3D12RendererApi : public IRendererApi
  {
  public:
    D3D12RendererApi();
    ~D3D12RendererApi();
  };
}