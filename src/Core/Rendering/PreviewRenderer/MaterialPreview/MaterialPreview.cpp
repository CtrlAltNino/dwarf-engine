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
    std::shared_ptr<IMeshFactory>        meshFactory,
    std::shared_ptr<IEditorStats>        editorStats)
    : PreviewRenderer(framebufferFactory,
                      cameraFactory->Create(),
                      rendererApiFactory,
                      editorStats)
    , m_Logger(logger)
    , m_MeshFactory(meshFactory)
  {
    FramebufferSpecification renderSpec = { 512, 512 };
    renderSpec.Samples = 8;
    renderSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    m_RenderFramebuffer = m_FramebufferFactory->Create(renderSpec);

    FramebufferSpecification previewSpec = { 512, 512 };
    previewSpec.Samples = 1;
    previewSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 }
    };
    m_PreviewFramebuffer = m_FramebufferFactory->Create(previewSpec);

    m_Camera->GetProperties().Fov = 50.0f;
    m_Camera->GetProperties().NearPlane = 0.1f;
    m_Camera->GetProperties().FarPlane = 25000.0f;
    m_Camera->GetProperties().AspectRatio = 1.0f;
    m_Camera->GetProperties().Transform.GetPosition() = { 0.0f, 0.0f, 0.0f };
    m_Camera->GetProperties().Transform.GetEulerAngles() = { 0, 0, 0 };

    m_Logger->LogInfo(Log("MaterialPreview created", "MaterialPreview"));
  }

  void
  MaterialPreview::RenderMaterialPreview(IMaterial& materialAsset)
  {
    // TODO: Reset sphere rotation when rendering a different material
    m_Camera->GetProperties().Transform.GetPosition() = { 0, 0, 3 };
    m_Camera->GetProperties().Transform.GetEulerAngles() = { 0, 0, 0 };
    m_Camera->GetProperties().NearPlane = 0.1f;
    m_Camera->GetProperties().FarPlane = 4;

    m_Properties.ModelRotation =
      InterpolateVectors(m_Properties.ModelRotation,
                         m_Properties.ModelRotationTarget,
                         m_Properties.RotationSpeed);
    m_Properties.ModelRotationQuat =
      glm::rotate(glm::rotate(glm::quat({ 0, 0, 0 }),
                              m_Properties.ModelRotation.x * DEG_2_RAD,
                              { 1, 0, 0 }),
                  m_Properties.ModelRotation.y * DEG_2_RAD,
                  { 0, 1, 0 });

    m_RenderFramebuffer->Bind();
    m_RendererApi->SetViewport(0,
                               0,
                               m_RenderFramebuffer->GetSpecification().Width,
                               m_RenderFramebuffer->GetSpecification().Height);
    m_RendererApi->SetClearColor({ 59 / 255.0f, 66 / 255.0f, 82 / 255.0f, 1 });
    m_RendererApi->Clear();

    m_RendererApi->RenderIndexed(m_Mesh,
                                 materialAsset,
                                 glm::toMat4(m_Properties.ModelRotationQuat),
                                 m_Camera->GetViewMatrix(),
                                 m_Camera->GetProjectionMatrix());
    m_RenderFramebuffer->Unbind();

    m_RendererApi->Blit(*m_RenderFramebuffer,
                        *m_PreviewFramebuffer,
                        0,
                        0,
                        m_RenderFramebuffer->GetSpecification().Width,
                        m_RenderFramebuffer->GetSpecification().Height);
  }

  void
  MaterialPreview::SetMeshType(MaterialPreviewMeshType meshType)
  {
    m_Logger->LogInfo(Log("Updating preview mesh", "MaterialPreview"));
    m_MeshType = meshType;
    UpdateMesh();
  }

  void
  MaterialPreview::UpdateMesh()
  {
    switch (m_MeshType)
    {
      case MaterialPreviewMeshType::Sphere:
        m_Mesh = m_MeshFactory->CreateUnitSphere(200, 200);
        break;
      case MaterialPreviewMeshType::Cube:
        m_Mesh = m_MeshFactory->CreateUnitCube();
        break;
      case MaterialPreviewMeshType::Plane:
        m_Mesh = m_MeshFactory->CreateUnitQuad();
        break;
    }
    m_Mesh->SetupMesh();
  }

  MaterialPreviewMeshType
  MaterialPreview::GetMeshType() const
  {
    return m_MeshType;
  }
}