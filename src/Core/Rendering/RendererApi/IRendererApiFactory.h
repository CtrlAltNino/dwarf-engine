#pragma once

#include "Core/Rendering/RendererApi/IRendererApi.h"

namespace Dwarf
{
  /**
   * @brief Class that creates renderer api instances depending on the used
   * graphics api in the project
   *
   */
  class IRendererApiFactory
  {
  public:
    virtual ~IRendererApiFactory() = default;

    /**
     * @brief Creates a renderer api instance
     *
     * @return std::shared_ptr<IRendererApi>
     */
    [[nodiscard]] virtual auto
    Create() const -> std::unique_ptr<IRendererApi> = 0;
  };
}