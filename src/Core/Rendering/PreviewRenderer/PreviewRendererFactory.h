#pragma once

#include "Core/Rendering/PreviewRenderer/IPreviewRendererFactory.h"

namespace Dwarf
{
  class PreviewRendererFactory : public IPreviewRendererFactory
  {
  public:
    std::shared_ptr<IMaterialPreview>
    CreateMaterialPreviewRenderer(
      std::shared_ptr<IFramebufferFactory> framebufferFactory,
      std::shared_ptr<ICamera>             camera,
      std::shared_ptr<IRendererApi>        rendererApi,
      std::shared_ptr<IMeshFactory>        meshFactory) override;

    std::shared_ptr<IModelPreview>
    CreateModelPreviewRenderer(
      std::shared_ptr<IFramebufferFactory> framebufferFactory,
      std::shared_ptr<ICamera>             camera,
      std::shared_ptr<IRendererApi>        rendererApi,
      std::shared_ptr<IMeshFactory>        meshFactory) override;
  };
}