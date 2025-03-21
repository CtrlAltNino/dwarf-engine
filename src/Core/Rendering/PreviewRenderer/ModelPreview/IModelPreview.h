#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Rendering/PreviewRenderer/PreviewRenderer.h"
#include <imgui.h>

namespace Dwarf
{
  /**
   * @brief A class that manages the preview of a model
   *
   */
  class IModelPreview : public virtual PreviewRenderer
  {
  public:
    virtual ~IModelPreview() = default;

    /**
     * @brief Renders the model asset as a preview
     *
     * @param modelAsset Model asset to preview
     */
    virtual void
    RenderModelPreview(IAssetReference& modelAsset) = 0;

    /**
     * @brief Positions the camera to fit the whole model in the camera view
     *
     * @param modelAsset Model asset to focus to
     */
    virtual void
    FocusModel(ModelAsset& modelAsset) = 0;
  };
}