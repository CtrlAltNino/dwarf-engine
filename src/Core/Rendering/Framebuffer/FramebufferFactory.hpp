#pragma once

#include "Core/Base.hpp"
#include "Core/Rendering/Texture/ITextureFactory.hpp"
#include "Core/Rendering/VramTracker/IVramTracker.hpp"
#include "IFramebufferFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

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
