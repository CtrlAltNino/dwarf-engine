#pragma once

#include "ICameraSystem.hpp"

namespace Dwarf
{
  /**
   * @brief Class interface defining the factory of ICameraSystem
   *
   */
  class ICameraSystemFactory
  {
  public:
    virtual ~ICameraSystemFactory() = default;

    /**
     * @brief Creates an instance of ICameraSystem
     *
     * @return std::shared_ptr<ICameraSystem> Shared pointer to the created
     * instance
     */
    virtual auto
    Create() -> std::shared_ptr<ICameraSystem> = 0;
  };
}