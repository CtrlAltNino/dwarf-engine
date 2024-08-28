#pragma once

#include "Core/Asset/Database/AssetComponents.h"
#include "Core/Asset/Database/AssetReference.h"
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
    RenderMaterialPreview(
      std::shared_ptr<AssetReference<MaterialAsset>> materialAsset) = 0;

    virtual void
    SetMeshType(MaterialPreviewMeshType meshType) = 0;
  };
}