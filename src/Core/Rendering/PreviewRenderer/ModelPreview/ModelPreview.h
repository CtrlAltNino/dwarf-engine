#pragma once

#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Core/Rendering/Material/IMaterial.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/PreviewRenderer/ModelPreview/IModelPreview.h"
#include "Core/Scene/Camera/ICameraFactory.h"

namespace Dwarf
{
  class ModelPreview : public IModelPreview
  {
  private:
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::unique_ptr<IMaterial>        m_Material;

  public:
    ModelPreview(std::shared_ptr<IFramebufferFactory> framebufferFactory,
                 std::shared_ptr<ICameraFactory>      cameraFactory,
                 std::shared_ptr<IRendererApiFactory> rendererApiFactory,
                 std::shared_ptr<IMaterialFactory>    materialFactory);

    void
    RenderModelPreview(IAssetReference<ModelAsset>& modelAsset) override;

    void
    FocusModel(IAssetReference<ModelAsset>& modelAsset) override;
  };
}