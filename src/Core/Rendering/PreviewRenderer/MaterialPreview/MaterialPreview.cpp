#include "pch.h"

#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Editor/Stats/IEditorStats.h"
#include "MaterialPreview.h"

namespace Dwarf
{
  MaterialPreview::MaterialPreview(
    std::shared_ptr<IDwarfLogger>               logger,
    std::shared_ptr<IFramebufferFactory>        framebufferFactory,
    const std::shared_ptr<ICameraFactory>&      cameraFactory,
    const std::shared_ptr<IRendererApiFactory>& rendererApiFactory,
    std::shared_ptr<IMeshBufferFactory>         meshBufferFactory,
    std::shared_ptr<IMeshFactory>               meshFactory,
    std::shared_ptr<IEditorStats>               editorStats)
    : PreviewRenderer(std::move(framebufferFactory),
                      cameraFactory->Create(),
                      rendererApiFactory,
                      std::move(editorStats))
    , mLogger(std::move(logger))
    , mMeshFactory(std::move(meshFactory))
    , mMeshBufferFactory(std::move(meshBufferFactory))
  {
    FramebufferSpecification renderSpec = { .Width = 512, .Height = 512 };
    renderSpec.Samples = 4;
    renderSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    mRenderFramebuffer = mFramebufferFactory->Create(renderSpec);

    FramebufferSpecification previewSpec = { .Width = 512, .Height = 512 };
    previewSpec.Samples = 1;
    previewSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 }
    };
    mPreviewFramebuffer = mFramebufferFactory->Create(previewSpec);

    mCamera->GetProperties().Fov = 50.0F;
    mCamera->GetProperties().NearPlane = 0.1F;
    mCamera->GetProperties().FarPlane = 25000.0F;
    mCamera->GetProperties().AspectRatio = 1.0F;
    mCamera->GetProperties().Transform.GetPosition() = { 0.0F, 0.0F, 0.0F };
    mCamera->GetProperties().Transform.GetEulerAngles() = { 0, 0, 0 };

    mLogger->LogDebug(Log("MaterialPreview created", "MaterialPreview"));
  }

  MaterialPreview::~MaterialPreview()
  {
    mLogger->LogDebug(Log("MaterialPreview destroyed", "MaterialPreview"));
  }

  void
  MaterialPreview::RenderMaterialPreview(IMaterial& material)
  {
    // TODO: Reset sphere rotation when rendering a different material
    mCamera->GetProperties().Transform.GetPosition() = { 0, 0, 3 };
    mCamera->GetProperties().Transform.GetEulerAngles() = { 0, 0, 0 };
    mCamera->GetProperties().NearPlane = 0.1F;
    mCamera->GetProperties().FarPlane = 4;

    mProperties.ModelRotation = mProperties.ModelRotationTarget;
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
    mRendererApi->SetClearColor({ 46 / 255.0F, 52 / 255.0F, 64 / 255.0F, 1 });
    mRendererApi->Clear();

    mRendererApi->RenderIndexed(*mMeshBuffer,
                                material,
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
    mProperties.ModelRotation = { 15, 20, 0 };
    mProperties.ModelRotationTarget = { 15, 20, 0 };
    UpdateRotation({ 0, 0 });
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
          std::unique_ptr<IMesh> mesh = mMeshFactory->CreatePreviewQuad();
          mMeshBuffer = mMeshBufferFactory->Create(mesh);
        }
        break;
    }
  }

  auto
  MaterialPreview::GetMeshType() const -> MaterialPreviewMeshType
  {
    return mMeshType;
  }
}