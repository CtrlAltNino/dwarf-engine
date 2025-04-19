#pragma once

#include "Core/Asset/AssetReference/IAssetReference.hpp"

namespace Dwarf
{
  /**
   * @brief Class that renders the inspector view of material assets
   *
   */
  class IMaterialAssetInspector
  {
  public:
    virtual ~IMaterialAssetInspector() = default;

    /**
     * @brief Renders the inspector for a material asset reference
     *
     * @param asset
     */
    virtual void
    Render(IAssetReference& asset) = 0;
  };
}