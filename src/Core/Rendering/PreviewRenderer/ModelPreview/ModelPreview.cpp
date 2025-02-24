#include "ModelPreview.h"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include <entt/entity/fwd.hpp>

namespace Dwarf
{
  ModelPreview::ModelPreview(
    std::shared_ptr<IFramebufferFactory> framebufferFactory,
    std::shared_ptr<ICameraFactory>      cameraFactory,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
    std::shared_ptr<IMaterialFactory>    materialFactory,
    std::shared_ptr<IEditorStats>        editorStats,
    std::shared_ptr<IInputManager>       inputManager)
    : PreviewRenderer(framebufferFactory,
                      cameraFactory->Create(),
                      rendererApiFactory,
                      editorStats)
    , m_MaterialFactory(materialFactory)
    , m_InputManager(inputManager)
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

    m_Material = m_MaterialFactory->CreateDefaultMaterial();
    m_Material->GetShader().Compile();
    m_Material->GenerateShaderParameters();
    m_Material->GetShaderParameters()->SetParameter("color",
                                                    glm::vec4(1, 1, 1, 1));
    m_Material->GetShaderParameters()->SetParameter("shininess", 32.0f);
  }

  void
  ModelPreview::RenderModelPreview(IAssetReference& modelAsset)
  {
    if (static entt::entity memory = entt::null;
        memory != modelAsset.GetHandle())
    {
      // std::cout << "ModelPreview::RenderModelPreview setup" << std::endl;
      FocusModel((ModelAsset&)modelAsset.GetAsset());
      memory = modelAsset.GetHandle();
      m_Properties.ModelRotation = { -15, 200, 0 };
      m_Properties.ModelRotationTarget = { -15, 200, 0 };
      UpdateRotation({ 0, 0 });
      m_Properties.Distance = 1.0f;
    }

    m_Properties.ModelRotation =
      InterpolateVectors(m_Properties.ModelRotation,
                         m_Properties.ModelRotationTarget,
                         m_Properties.RotationSpeed);
    m_Properties.ModelRotation = m_Properties.ModelRotationTarget;
    m_Properties.ModelRotationQuat =
      glm::rotate(glm::rotate(glm::quat({ 0, 0, 0 }),
                              m_Properties.ModelRotation.x * DEG_2_RAD,
                              { 1, 0, 0 }),
                  m_Properties.ModelRotation.y * DEG_2_RAD,
                  { 0, 1, 0 });

    // m_Properties.ModelRotationQuat = glm::quat({ 0, 0, 0 });

    ModelAsset& modelAssetRef = (ModelAsset&)modelAsset.GetAsset();

    m_Camera->GetProperties().Transform.GetPosition() = InterpolateVectors(
      m_Camera->GetProperties().Transform.GetPosition(),
      { 0, 0, 1.3 * m_Properties.MaxDistance * m_Properties.Distance },
      m_Properties.ScrollSpeed);

    m_Camera->GetProperties().NearPlane = 0.1f;
    m_Camera->GetProperties().FarPlane =
      1.3 * m_Properties.MaxDistance + m_Properties.MaxDistance;

    // m_Camera->GetProperties().Transform.GetPosition() = { 0, 0, 35 };

    m_RenderFramebuffer->Bind();
    m_RendererApi->SetViewport(0,
                               0,
                               m_RenderFramebuffer->GetSpecification().Width,
                               m_RenderFramebuffer->GetSpecification().Height);
    m_RendererApi->SetClearColor({ 59 / 255.0f, 66 / 255.0f, 82 / 255.0f, 1 });
    m_RendererApi->Clear();

    for (int i = 0; i < modelAssetRef.Meshes().size(); i++)
    {
      m_RendererApi->RenderIndexed(modelAssetRef.Meshes().at(i),
                                   *m_Material,
                                   *m_Camera,
                                   glm::toMat4(m_Properties.ModelRotationQuat));
    }
    m_RenderFramebuffer->Unbind();

    m_RendererApi->Blit(*m_RenderFramebuffer,
                        *m_PreviewFramebuffer,
                        0,
                        0,
                        m_RenderFramebuffer->GetSpecification().Width,
                        m_RenderFramebuffer->GetSpecification().Height);
  }

  void
  ModelPreview::FocusModel(ModelAsset& modelAsset)
  {
    float longestDist = 0;

    for (const auto& subMesh : modelAsset.Meshes())
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
    m_Camera->GetProperties().Transform.GetPosition() = {
      0, 0, 1.3 * m_Properties.MaxDistance * m_Properties.Distance
    };
    m_Camera->GetProperties().NearPlane = 0.1f;
    m_Camera->GetProperties().NearPlane =
      1.3 * m_Properties.MaxDistance + longestDist;
  }
}