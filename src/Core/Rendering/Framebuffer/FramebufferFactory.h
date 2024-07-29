#pragma once

#include "Core/Base.h"
#include "IFramebufferFactory.h"

namespace Dwarf
{
  class FramebufferFactory : public IFramebufferFactory
  {
  public:
    FramebufferFactory(GraphicsApi api);

    virtual std::shared_ptr<IFramebuffer>
    Create(const FramebufferSpecification& spec) override;

  private:
    GraphicsApi m_Api;
  };
}
