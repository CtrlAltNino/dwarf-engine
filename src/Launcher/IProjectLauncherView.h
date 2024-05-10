#pragma once

namespace Dwarf
{
  /**
   * @brief Interface for the project launcher view
   */
  class IProjectLauncherView
  {
  public:
    virtual ~IProjectLauncherView() = default;

    /**
     * @brief Render the project launcher view
     */
    virtual void
    Render() = 0;
  };
}