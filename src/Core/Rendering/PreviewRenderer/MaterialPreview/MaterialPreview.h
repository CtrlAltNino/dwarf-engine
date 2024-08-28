#pragma once

#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"

namespace Dwarf
{
  class MaterialPreview : public IMaterialPreview
  {
  private:
    MaterialPreviewMeshType       m_MeshType = MaterialPreviewMeshType::Sphere;
    std::shared_ptr<IMeshFactory> m_MeshFactory;
    std::shared_ptr<IMesh>        m_Mesh;

    void
    UpdateMesh();

  public:
    MaterialPreview(std::shared_ptr<IFramebufferFactory> framebufferFactory,
                    std::shared_ptr<ICamera>             camera,
                    std::shared_ptr<IRendererApi>        rendererApi,
                    std::shared_ptr<IMeshFactory>        meshFactory);

    void
    RenderMaterialPreview(
      std::shared_ptr<AssetReference<MaterialAsset>> materialAsset) override;

    void
    SetMeshType(MaterialPreviewMeshType meshType) override;
  };
}