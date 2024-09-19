#pragma once

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Rendering/PreviewRenderer/PreviewRenderer.h"
#include <imgui.h>
namespace Dwarf
{
  enum class MaterialPreviewMeshType
  {
    Sphere,
    Cube,
    Plane
  };
  class IMaterialPreview : public virtual PreviewRenderer
  {
  public:
    virtual void
    RenderMaterialPreview(IMaterial& materialAsset) = 0;

    virtual void
    SetMeshType(MaterialPreviewMeshType meshType) = 0;
  };
}