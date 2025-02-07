#pragma once

#include "Core/Rendering/RendererApi/IRendererApi.h"
namespace Dwarf
{
  class IRendererApiFactory
  {
  public:
    virtual ~IRendererApiFactory() = default;
    virtual std::shared_ptr<IRendererApi>
    Create() = 0;
  };
}