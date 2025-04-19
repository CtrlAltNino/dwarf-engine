#pragma once

#include "Core/Asset/AssetReference/IAssetReference.hpp"

namespace Dwarf
{
  /**
   * @brief Class that renders the inspector for scene assets
   *
   */
  class ISceneAssetInspector
  {
  public:
    virtual ~ISceneAssetInspector() = default;

    /**
     * @brief Renders the scene asset inspector
     *
     * @param asset
     */
    virtual void
    Render(IAssetReference& asset) = 0;
  };
}