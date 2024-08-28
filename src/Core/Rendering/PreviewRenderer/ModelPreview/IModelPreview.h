#pragma once

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/AssetReference.h"
#include "Core/Rendering/PreviewRenderer/PreviewRenderer.h"
#include <imgui.h>

namespace Dwarf
{
  class IModelPreview : public virtual PreviewRenderer
  {
  public:
    virtual void
    RenderModelPreview(
      std::shared_ptr<AssetReference<ModelAsset>> modelAsset) = 0;

    virtual void
    FocusModel(std::shared_ptr<AssetReference<ModelAsset>> modelAsset) = 0;
  };
}