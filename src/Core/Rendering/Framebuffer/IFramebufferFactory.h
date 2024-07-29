#pragma once

#include "IFramebuffer.h"

namespace Dwarf
{
  class IFramebufferFactory
  {
  public:
    virtual ~IFramebufferFactory() = default;

    virtual std::shared_ptr<IFramebuffer>
    Create(const FramebufferSpecification& spec) = 0;
  };
}