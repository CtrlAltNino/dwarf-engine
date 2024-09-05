#include "ModelPreview.h"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"

namespace Dwarf
{
  ModelPreview::ModelPreview(
    std::shared_ptr<IFramebufferFactory> framebufferFactory,
    std::shared_ptr<ICameraFactory>      cameraFactory,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
    std::shared_ptr<IMaterialFactory>    materialFactory)
    : PreviewRenderer(framebufferFactory,
                      cameraFactory->Create(),
                      rendererApiFactory)
    , m_MaterialFactory(materialFactory)
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

    m_Material = m_MaterialFactory->CreateDefaultMaterial();
  }
  void
  ModelPreview::RenderModelPreview(
    std::shared_ptr<AssetReference<ModelAsset>> modelAsset)
  {
    if (static entt::entity memory = entt::null;
        memory != modelAsset->GetHandle())
    {
      FocusModel(modelAsset);
      memory = modelAsset->GetHandle();
      m_Properties.ModelRotation = { 15, 20, 0 };
      m_Properties.ModelRotationTarget = { 15, 20, 0 };
      UpdateRotation({ 0, 0 });
      m_Properties.Distance = 1.0f;
    }

    m_Camera->GetProperties().Transform.position = InterpolateVectors(
      m_Camera->GetProperties().Transform.position,
      { 0, 0, 1.3 * m_Properties.MaxDistance * m_Properties.Distance },
      m_Properties.ScrollSpeed);

    m_Framebuffer->Bind();
    m_RendererApi->SetClearColor({ 59 / 255.0f, 66 / 255.0f, 82 / 255.0f, 1 });
    m_RendererApi->Clear();
    m_RendererApi->SetViewport(0,
                               0,
                               m_Framebuffer->GetSpecification().Width,
                               m_Framebuffer->GetSpecification().Height);

    for (int i = 0; i < modelAsset->GetAsset()->m_Meshes.size(); i++)
    {
      m_RendererApi->RenderIndexed(modelAsset->GetAsset()->m_Meshes.at(i),
                                   m_Material,
                                   glm::toMat4(m_Properties.ModelRotationQuat),
                                   m_Camera->GetViewMatrix(),
                                   m_Camera->GetProjectionMatrix());
    }
    m_Framebuffer->Unbind();
  }

  void
  ModelPreview::FocusModel(
    std::shared_ptr<AssetReference<ModelAsset>> modelAsset)
  {
    std::vector<std::shared_ptr<IMesh>> vec = modelAsset->GetAsset()->m_Meshes;
    float                               longestDist = 0;

    for (const auto& subMesh : vec)
    {
      for (Vertex vert : subMesh->GetVertices())
      {
        float dist = glm::length(vert.Position);
        if (dist > longestDist)
        {
          longestDist = dist;
        }
      }
    }

    m_Properties.MaxDistance =
      longestDist / (tanf(0.5f * m_Camera->GetProperties().Fov * DEG_2_RAD));
    m_Camera->GetProperties().Transform.position = {
      0, 0, 1.3 * m_Properties.MaxDistance * m_Properties.Distance
    };
    m_Camera->GetProperties().NearPlane = 0.1f;
    m_Camera->GetProperties().NearPlane =
      1.3 * m_Properties.MaxDistance + longestDist;
  }
}