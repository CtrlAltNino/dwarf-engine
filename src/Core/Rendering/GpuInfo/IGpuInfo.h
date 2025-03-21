#pragma once

#include <cstddef>
namespace Dwarf
{
  /**
   * @brief A class that provides information on the used GPU device
   *
   */
  class IGpuInfo
  {
  public:
    virtual ~IGpuInfo() = default;

    /**
     * @brief Get the amount of used VRAM
     *
     * @return The amount of used VRAM in MB
     */
    virtual auto
    GetUsedVramMb() -> size_t = 0;

    /**
     * @brief Get the amount of total VRAM
     *
     * @return The amount of total VRAM in MB
     */
    virtual auto
    GetTotalVramMb() -> size_t = 0;
  };
}