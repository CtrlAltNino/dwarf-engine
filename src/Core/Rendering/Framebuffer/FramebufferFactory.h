#pragma once

#include "Core/Base.h"
#include "IFramebufferFactory.h"
#include "Core/Rendering/Texture/ITextureFactory.h"

namespace Dwarf
{
  class FramebufferFactory : public IFramebufferFactory
  {
  public:
    FramebufferFactory(GraphicsApi                      api,
                       std::shared_ptr<ITextureFactory> textureFactory);
    ~FramebufferFactory() override = default;

    virtual std::shared_ptr<IFramebuffer>
    Create(const FramebufferSpecification& spec) override;

  private:
    GraphicsApi                      m_Api;
    std::shared_ptr<ITextureFactory> m_TextureFactory;
  };
}
