#pragma once

#include "MaterialPreview/IMaterialPreview.h"
#include "ModelPreview/IModelPreview.h"
namespace Dwarf
{
  class IPreviewRendererFactory
  {
  public:
    virtual std::shared_ptr<IMaterialPreview>
    CreateMaterialPreviewRenderer() = 0;

    virtual std::shared_ptr<IModelPreview>
    CreateModelPreviewRenderer() = 0;
  };
}