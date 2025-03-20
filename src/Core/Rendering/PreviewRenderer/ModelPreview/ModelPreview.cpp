#include "ModelPreview.h"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include <entt/entity/fwd.hpp>

namespace Dwarf
{
  ModelPreview::ModelPreview(
    std::shared_ptr<IFramebufferFactory> framebufferFactory,
    std::shared_ptr<ICameraFactory>      cameraFactory,
    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
    std::shared_ptr<IMaterialFactory>    materialFactory,
    std::shared_ptr<IEditorStats>        editorStats,
    std::shared_ptr<IInputManager>       inputManager,
    std::shared_ptr<IMeshFactory>        meshFactory,
    std::shared_ptr<IMeshBufferFactory>  meshBufferFactory)
    : PreviewRenderer(framebufferFactory,
                      cameraFactory->Create(),
                      rendererApiFactory,
                      editorStats)
    , mMaterialFactory(materialFactory)
    , mInputManager(inputManager)
    , mMeshFactory(meshFactory)
    , mMeshBufferFactory(meshBufferFactory)
  {
    FramebufferSpecification renderSpec = { 512, 512 };
    renderSpec.Samples = 8;
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

    mMaterial = mMaterialFactory->CreateDefaultMaterial();
    mMaterial->GetShader().Compile();
    mMaterial->GenerateShaderParameters();
    mMaterial->GetShaderParameters()->SetParameter("color",
                                                   glm::vec4(1, 1, 1, 1));
    mMaterial->GetShaderParameters()->SetParameter("shininess", 32.0f);
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
      mProperties.ModelRotation = { -15, 200, 0 };
      mProperties.ModelRotationTarget = { -15, 200, 0 };
      UpdateRotation({ 0, 0 });
      mProperties.Distance = 1.0f;
      std::unique_ptr<IMesh> mesh = mMeshFactory->MergeMeshes(
        ((ModelAsset&)modelAsset.GetAsset()).Meshes());
      mPreviewMeshBuffer = std::move(mMeshBufferFactory->Create(mesh));
    }

    mProperties.ModelRotation =
      InterpolateVectors(mProperties.ModelRotation,
                         mProperties.ModelRotationTarget,
                         mProperties.RotationSpeed);
    mProperties.ModelRotation = mProperties.ModelRotationTarget;
    mProperties.ModelRotationQuat =
      glm::rotate(glm::rotate(glm::quat({ 0, 0, 0 }),
                              mProperties.ModelRotation.x * DEG_2_RAD,
                              { 1, 0, 0 }),
                  mProperties.ModelRotation.y * DEG_2_RAD,
                  { 0, 1, 0 });

    // mProperties.ModelRotationQuat = glm::quat({ 0, 0, 0 });

    ModelAsset& modelAssetRef = (ModelAsset&)modelAsset.GetAsset();

    mCamera->GetProperties().Transform.GetPosition() = InterpolateVectors(
      mCamera->GetProperties().Transform.GetPosition(),
      { 0, 0, 1.3 * mProperties.MaxDistance * mProperties.Distance },
      mProperties.ScrollSpeed);

    mCamera->GetProperties().NearPlane = 0.1f;
    mCamera->GetProperties().FarPlane =
      1.3 * mProperties.MaxDistance + mProperties.MaxDistance;

    // mCamera->GetProperties().Transform.GetPosition() = { 0, 0, 35 };

    mRenderFramebuffer->Bind();
    mRendererApi->SetViewport(0,
                              0,
                              mRenderFramebuffer->GetSpecification().Width,
                              mRenderFramebuffer->GetSpecification().Height);
    mRendererApi->SetClearColor({ 59 / 255.0f, 66 / 255.0f, 82 / 255.0f, 1 });
    mRendererApi->Clear();

    if (mPreviewMeshBuffer)
    {
      mRendererApi->RenderIndexed(*mPreviewMeshBuffer,
                                  *mMaterial,
                                  *mCamera,
                                  glm::toMat4(mProperties.ModelRotationQuat));
    }

    // for (int i = 0; i < modelAssetRef.Meshes().size(); i++)
    // {
    //   mRendererApi->RenderIndexed(*modelAssetRef.Meshes().at(i),
    //                                *mMaterial,
    //                                *mCamera,
    //                                glm::toMat4(mProperties.ModelRotationQuat));
    // }
    mRenderFramebuffer->Unbind();

    mRendererApi->Blit(*mRenderFramebuffer,
                       *mPreviewFramebuffer,
                       0,
                       0,
                       mRenderFramebuffer->GetSpecification().Width,
                       mRenderFramebuffer->GetSpecification().Height);
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

    mProperties.MaxDistance =
      longestDist / (tanf(0.5f * mCamera->GetProperties().Fov * DEG_2_RAD));
    mCamera->GetProperties().Transform.GetPosition() = {
      0, 0, 1.3 * mProperties.MaxDistance * mProperties.Distance
    };
    mCamera->GetProperties().NearPlane = 0.1f;
    mCamera->GetProperties().NearPlane =
      1.3 * mProperties.MaxDistance + longestDist;
  }
}