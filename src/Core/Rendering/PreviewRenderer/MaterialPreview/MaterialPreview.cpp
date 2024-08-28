#include "MaterialPreview.h"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"

namespace Dwarf
{
  MaterialPreview::MaterialPreview(
    std::shared_ptr<IFramebufferFactory> framebufferFactory,
    std::shared_ptr<ICamera>             camera,
    std::shared_ptr<IRendererApi>        rendererApi,
    std::shared_ptr<IMeshFactory>        meshFactory)
    : PreviewRenderer(framebufferFactory, camera, rendererApi)
    , m_MeshFactory(meshFactory)
  {
    FramebufferSpecification spec = { 512, 512 };
    spec.Samples = 8;
    spec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA8 },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    m_Framebuffer = m_FramebufferFactory->Create(spec);

    m_Camera->GetProperties().Fov = 50.0f;
    m_Camera->GetProperties().NearPlane = 0.1f;
    m_Camera->GetProperties().FarPlane = 25000.0f;
    m_Camera->GetProperties().AspectRatio = 1.0f;
    m_Camera->GetProperties().Transform.position = { 0.0f, 0.0f, 0.0f };
    m_Camera->GetProperties().Transform.rotation = { 0, 0, 0 };

    UpdateMesh();
  }

  void
  MaterialPreview::RenderMaterialPreview(
    std::shared_ptr<AssetReference<MaterialAsset>> materialAsset)
  {
    // TODO: Reset sphere rotation when rendering a different material
    m_Camera->GetProperties().Transform.position = { 0, 0, 3 };
    m_Camera->GetProperties().Transform.rotation = { 0, 0, 0 };
    m_Camera->GetProperties().NearPlane = 0.1f;
    m_Camera->GetProperties().FarPlane = 4;

    m_Framebuffer->Bind();
    m_RendererApi->SetClearColor({ 59 / 255.0f, 66 / 255.0f, 82 / 255.0f, 1 });
    m_RendererApi->Clear();
    m_RendererApi->SetViewport(0,
                               0,
                               m_Framebuffer->GetSpecification().Width,
                               m_Framebuffer->GetSpecification().Height);

    m_RendererApi->RenderIndexed(m_Mesh,
                                 materialAsset->GetAsset()->m_Material,
                                 glm::toMat4(m_Properties.ModelRotationQuat),
                                 m_Camera->GetViewMatrix(),
                                 m_Camera->GetProjectionMatrix());
    m_Framebuffer->Unbind();
  }

  void
  MaterialPreview::SetMeshType(MaterialPreviewMeshType meshType)
  {
    m_MeshType = meshType;
    UpdateMesh();
  }

  void
  MaterialPreview::UpdateMesh()
  {
    switch (m_MeshType)
    {
      case MaterialPreviewMeshType::Sphere:
        m_Mesh = m_MeshFactory->CreateUnitSphere(20, 20);
        break;
      case MaterialPreviewMeshType::Cube:
        m_Mesh = m_MeshFactory->CreateUnitCube();
        break;
      case MaterialPreviewMeshType::Plane:
        m_Mesh = m_MeshFactory->CreateUnitQuad();
        break;
    }
  }
}