#pragma once

#include "IFramebuffer.h"

namespace Dwarf
{
  /**
   * @brief Factory to create framebuffers depending on the used graphics api
   *
   */
  class IFramebufferFactory
  {
  public:
    virtual ~IFramebufferFactory() = default;

    /**
     * @brief Creates a framebuffer according to a given specification
     *
     * @param spec Specification of a framebuffer
     * @return The created framebuffer
     */
    virtual auto
    Create(const FramebufferSpecification& spec)
      -> std::unique_ptr<IFramebuffer> = 0;
  };
}