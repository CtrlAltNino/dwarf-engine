#pragma once

namespace Dwarf
{
  /**
   * @brief Class that manages rendering of selected assets to the inspector
   *
   */
  class IAssetInspector
  {
  public:
    virtual ~IAssetInspector() = default;

    /**
     * @brief Renders the Inspector for assets
     *
     */
    virtual void
    Render() = 0;
  };
}