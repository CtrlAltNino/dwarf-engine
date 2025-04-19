#pragma once

#include "Core/Rendering/Framebuffer/IFramebuffer.hpp"
#include "IPingPongBuffer.hpp"

namespace Dwarf
{
  /**
   * @brief Class that creates PingPongBuffer instances
   *
   */
  class IPingPongBufferFactory
  {
  public:
    virtual ~IPingPongBufferFactory() = default;

    /**
     * @brief Creates a ping pong buffer according to the specification
     *
     * @param spec Framebuffer specification to use
     * @return Unique pointer to the created instance
     */
    [[nodiscard]] virtual auto
    Create(FramebufferSpecification spec)
      -> std::unique_ptr<IPingPongBuffer> = 0;
  };
}