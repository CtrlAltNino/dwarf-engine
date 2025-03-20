#include "MaterialPreview.h"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Editor/Stats/IEditorStats.h"

namespace Dwarf
{
  MaterialPreview::MaterialPreview(
    std::shared_ptr<IDwarfLogger>        logger,
    std::shared_ptr<IFramebufferFactory> framebufferFactory,
    std::shared_ptr<ICameraFactory>      cameraFactory,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
    std::shared_ptr<IMeshBufferFactory>  meshBufferFactory,
    std::shared_ptr<IMeshFactory>        meshFactory,
    std::shared_ptr<IEditorStats>        editorStats)
    : PreviewRenderer(framebufferFactory,
                      cameraFactory->Create(),
                      rendererApiFactory,
                      editorStats)
    , mLogger(logger)
    , mMeshFactory(meshFactory)
    , mMeshBufferFactory(meshBufferFactory)
  {
    FramebufferSpecification renderSpec = { 512, 512 };
    renderSpec.Samples = 4;
    renderSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    mRenderFramebuffer = mFramebufferFactory->Create(renderSpec);

    FramebufferSpecification previewSpec = { 512, 512 };
    previewSpec.Samples = 1;
    previewSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 }
    };
    mPreviewFramebuffer = mFramebufferFactory->Create(previewSpec);

    mCamera->GetProperties().Fov = 50.0f;
    mCamera->GetProperties().NearPlane = 0.1f;
    mCamera->GetProperties().FarPlane = 25000.0f;
    mCamera->GetProperties().AspectRatio = 1.0f;
    mCamera->GetProperties().Transform.GetPosition() = { 0.0f, 0.0f, 0.0f };
    mCamera->GetProperties().Transform.GetEulerAngles() = { 0, 0, 0 };

    mLogger->LogDebug(Log("MaterialPreview created", "MaterialPreview"));
  }

  void
  MaterialPreview::RenderMaterialPreview(IMaterial& materialAsset)
  {
    // TODO: Reset sphere rotation when rendering a different material
    mCamera->GetProperties().Transform.GetPosition() = { 0, 0, 3 };
    mCamera->GetProperties().Transform.GetEulerAngles() = { 0, 0, 0 };
    mCamera->GetProperties().NearPlane = 0.1f;
    mCamera->GetProperties().FarPlane = 4;

    mProperties.ModelRotation =
      InterpolateVectors(mProperties.ModelRotation,
                         mProperties.ModelRotationTarget,
                         mProperties.RotationSpeed);
    mProperties.ModelRotationQuat =
      glm::rotate(glm::rotate(glm::quat({ 0, 0, 0 }),
                              mProperties.ModelRotation.x * DEG_2_RAD,
                              { 1, 0, 0 }),
                  mProperties.ModelRotation.y * DEG_2_RAD,
                  { 0, 1, 0 });

    mRenderFramebuffer->Bind();
    mRendererApi->SetViewport(0,
                              0,
                              mRenderFramebuffer->GetSpecification().Width,
                              mRenderFramebuffer->GetSpecification().Height);
    mRendererApi->SetClearColor({ 59 / 255.0f, 66 / 255.0f, 82 / 255.0f, 1 });
    mRendererApi->Clear();

    mRendererApi->RenderIndexed(*mMeshBuffer,
                                materialAsset,
                                *mCamera,
                                glm::toMat4(mProperties.ModelRotationQuat));
    mRenderFramebuffer->Unbind();

    mRendererApi->Blit(*mRenderFramebuffer,
                       *mPreviewFramebuffer,
                       0,
                       0,
                       mRenderFramebuffer->GetSpecification().Width,
                       mRenderFramebuffer->GetSpecification().Height);
  }

  void
  MaterialPreview::SetMeshType(MaterialPreviewMeshType meshType)
  {
    mLogger->LogDebug(Log("Updating preview mesh", "MaterialPreview"));
    mMeshType = meshType;
    UpdateMesh();
  }

  void
  MaterialPreview::UpdateMesh()
  {
    switch (mMeshType)
    {
      case MaterialPreviewMeshType::Sphere:
        {
          std::unique_ptr<IMesh> mesh = mMeshFactory->CreateUnitSphere(50, 50);
          mMeshBuffer = mMeshBufferFactory->Create(mesh);
          break;
        }
      case MaterialPreviewMeshType::Cube:
        {
          std::unique_ptr<IMesh> mesh = mMeshFactory->CreateUnitCube();
          mMeshBuffer = mMeshBufferFactory->Create(mesh);
        }
        break;
      case MaterialPreviewMeshType::Plane:
        {
          std::unique_ptr<IMesh> mesh = mMeshFactory->CreateUnitQuad();
          mMeshBuffer = mMeshBufferFactory->Create(mesh);
        }
        break;
    }
  }

  MaterialPreviewMeshType
  MaterialPreview::GetMeshType() const
  {
    return mMeshType;
  }
}