#pragma once

#include "Core/Base.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "IFramebufferFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class FramebufferFactory : public IFramebufferFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>    mLogger;
    GraphicsApi                      mApi;
    std::shared_ptr<ITextureFactory> mTextureFactory;
    std::shared_ptr<IVramTracker>    mVramTracker;

  public:
    FramebufferFactory(std::shared_ptr<IDwarfLogger>    logger,
                       GraphicsApi                      api,
                       std::shared_ptr<ITextureFactory> textureFactory,
                       std::shared_ptr<IVramTracker>    vramTracker);
    ~FramebufferFactory() override = default;

    virtual std::shared_ptr<IFramebuffer>
    Create(const FramebufferSpecification& spec) override;
  };
}
