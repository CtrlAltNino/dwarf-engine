#pragma once

#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Scene/Camera/ICameraFactory.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class MaterialPreview : public IMaterialPreview
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    MaterialPreviewMeshType       mMeshType = MaterialPreviewMeshType::Sphere;
    std::shared_ptr<IMeshBufferFactory> mMeshBufferFactory;
    std::shared_ptr<IMeshFactory>       mMeshFactory;
    std::unique_ptr<IMeshBuffer>        mMeshBuffer;

    void
    UpdateMesh();

  public:
    MaterialPreview(std::shared_ptr<IDwarfLogger>        logger,
                    std::shared_ptr<IFramebufferFactory> framebufferFactory,
                    std::shared_ptr<ICameraFactory>      cameraFactory,
                    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
                    std::shared_ptr<IMeshBufferFactory>  meshBufferFactory,
                    std::shared_ptr<IMeshFactory>        meshFactory,
                    std::shared_ptr<IEditorStats>        editorStats);

    void
    RenderMaterialPreview(IMaterial& materialAsset) override;

    void
    SetMeshType(MaterialPreviewMeshType meshType) override;

    MaterialPreviewMeshType
    GetMeshType() const override;
  };
}