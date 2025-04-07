#pragma once

#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "IPingPongBufferFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  /**
   * @brief Class that creates PingPongBuffer instances
   *
   */
  class PingPongBufferFactory : public IPingPongBufferFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>        mLogger;
    std::shared_ptr<IFramebufferFactory> mFramebufferFactory;

  public:
    PingPongBufferFactory(
      std::shared_ptr<IDwarfLogger>        logger,
      std::shared_ptr<IFramebufferFactory> framebufferFactory);
    ~PingPongBufferFactory() override;

    /**
     * @brief Creates a ping pong buffer according to the specification
     *
     * @param spec Framebuffer specification to use
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] auto
    Create(FramebufferSpecification spec)
      -> std::unique_ptr<IPingPongBuffer> override;
  };
}