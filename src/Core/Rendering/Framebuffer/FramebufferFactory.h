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
    FramebufferFactory(GraphicsApi                      api,
                       std::shared_ptr<IDwarfLogger>    logger,
                       std::shared_ptr<ITextureFactory> textureFactory,
                       std::shared_ptr<IVramTracker>    vramTracker);
    ~FramebufferFactory() override = default;

    /**
     * @brief Creates a framebuffer according to a given specification
     *
     * @param spec Specification of a framebuffer
     * @return The created framebuffer
     */
    auto
    Create(const FramebufferSpecification& spec)
      -> std::unique_ptr<IFramebuffer> override;
  };
}
