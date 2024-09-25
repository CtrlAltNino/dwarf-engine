#pragma once

#include "Core/Base.h"
#include "IFramebufferFactory.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class FramebufferFactory : public IFramebufferFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>    m_Logger;
    GraphicsApi                      m_Api;
    std::shared_ptr<ITextureFactory> m_TextureFactory;

  public:
    FramebufferFactory(std::shared_ptr<IDwarfLogger>    logger,
                       GraphicsApi                      api,
                       std::shared_ptr<ITextureFactory> textureFactory);
    ~FramebufferFactory() override = default;

    virtual std::shared_ptr<IFramebuffer>
    Create(const FramebufferSpecification& spec) override;
  };
}
