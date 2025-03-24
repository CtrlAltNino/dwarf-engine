#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"

namespace Dwarf
{
  /**
   * @brief Class that renders the inspector of a texture asset
   *
   */
  class ITextureAssetInspector
  {
  public:
    virtual ~ITextureAssetInspector() = default;

    /**
     * @brief Renders the texture asset inspector
     *
     * @param asset
     */
    virtual void
    Render(IAssetReference& asset) = 0;
  };
}