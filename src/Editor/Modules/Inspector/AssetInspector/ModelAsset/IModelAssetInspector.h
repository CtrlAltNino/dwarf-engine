#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
namespace Dwarf
{
  /**
   * @brief Class that renders the inspector for a model asset
   *
   */
  class IModelAssetInspector
  {
  public:
    virtual ~IModelAssetInspector() = default;

    /**
     * @brief Renders the model asset inspector
     *
     * @param asset
     */
    virtual void
    Render(IAssetReference& asset) = 0;
  };
}