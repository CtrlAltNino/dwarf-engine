#pragma once

#include "Core/Base.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "IFramebufferFactory.h"

namespace Dwarf
{
  class FramebufferFactory : public IFramebufferFactory
  {
  public:
    FramebufferFactory(GraphicsApi                      api,
                       std::shared_ptr<ITextureFactory> textureFactory);

    virtual std::shared_ptr<IFramebuffer>
    Create(const FramebufferSpecification& spec) override;

  private:
    GraphicsApi                      m_Api;
    std::shared_ptr<ITextureFactory> m_TextureFactory;
  };
}
