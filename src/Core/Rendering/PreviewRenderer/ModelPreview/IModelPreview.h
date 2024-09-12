#pragma once

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Rendering/PreviewRenderer/PreviewRenderer.h"
#include <imgui.h>

namespace Dwarf
{
  class IModelPreview : public virtual PreviewRenderer
  {
  public:
    virtual void
    RenderModelPreview(IAssetReference<ModelAsset>& modelAsset) = 0;

    virtual void
    FocusModel(IAssetReference<ModelAsset>& modelAsset) = 0;
  };
}