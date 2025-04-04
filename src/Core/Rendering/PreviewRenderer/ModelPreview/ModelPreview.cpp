#include "pch.h"

#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "ModelPreview.h"

namespace Dwarf
{
  ModelPreview::ModelPreview(
    std::shared_ptr<IDwarfLogger>               logger,
    std::shared_ptr<IFramebufferFactory>        framebufferFactory,
    const std::shared_ptr<ICameraFactory>&      cameraFactory,
    const std::shared_ptr<IRendererApiFactory>& rendererApiFactory,
    const std::shared_ptr<IMaterialFactory>&    materialFactory,
    std::shared_ptr<IEditorStats>               editorStats,
    std::shared_ptr<IInputManager>              inputManager,
    std::shared_ptr<IMeshFactory>               meshFactory,
    std::shared_ptr<IMeshBufferFactory>         meshBufferFactory,
    std::shared_ptr<IShaderSourceCollectionFactory>
      shaderSourceCollectionFactory)
    : PreviewRenderer(std::move(framebufferFactory),
                      cameraFactory->Create(),
                      rendererApiFactory,
                      std::move(editorStats))
    , mLogger(std::move(logger))
    , mInputManager(std::move(inputManager))
    , mMeshFactory(std::move(meshFactory))
    , mMeshBufferFactory(std::move(meshBufferFactory))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
  {
    FramebufferSpecification renderSpec = { .Width = 512, .Height = 512 };
    renderSpec.Samples = std::max(2, mRendererApi->GetMaxSamples());
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

    mMaterial = materialFactory->CreateMaterial(
      mShaderSourceCollectionFactory->CreatePreviewShaderSourceCollection());
    mMaterial->GetShader()->Compile();
    mMaterial->GenerateShaderParameters();
    mLogger->LogDebug(Log("ModelPreview destroyed", "ModelPreview"));
  }

  ModelPreview::~ModelPreview()
  {
    mLogger->LogDebug(Log("ModelPreview destroyed", "ModelPreview"));
  }

  void
  ModelPreview::RenderModelPreview(IAssetReference& modelAsset)
  {
    if (static entt::entity memory = entt::null;
        memory != modelAsset.GetHandle())
    {
      FocusModel(dynamic_cast<ModelAsset&>(modelAsset.GetAsset()));
      memory = modelAsset.GetHandle();
      mProperties.ModelRotation = { 15, 20, 0 };
      mProperties.ModelRotationTarget = { 15, 20, 0 };
      UpdateRotation({ 0, 0 });
      mProperties.Distance = 1.0F;
      std::unique_ptr<IMesh> mesh = mMeshFactory->MergeMeshes(
        (dynamic_cast<ModelAsset&>(modelAsset.GetAsset())).Meshes());
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

    auto& modelAssetRef = dynamic_cast<ModelAsset&>(modelAsset.GetAsset());

    mCamera->GetProperties().Transform.GetPosition() = InterpolateVectors(
      mCamera->GetProperties().Transform.GetPosition(),
      { 0, 0, 1.3F * mProperties.MaxDistance * mProperties.Distance },
      mProperties.ScrollSpeed);

    mCamera->GetProperties().NearPlane = 0.1F;
    mCamera->GetProperties().FarPlane =
      1.3F * mProperties.MaxDistance + mProperties.MaxDistance;

    // mCamera->GetProperties().Transform.GetPosition() = { 0, 0, 35 };

    mRenderFramebuffer->Bind();
    mRendererApi->SetViewport(0,
                              0,
                              mRenderFramebuffer->GetSpecification().Width,
                              mRenderFramebuffer->GetSpecification().Height);
    mRendererApi->SetClearColor({ 59 / 255.0F, 66 / 255.0F, 82 / 255.0F, 1 });
    mRendererApi->Clear();

    if (mPreviewMeshBuffer)
    {
      mRendererApi->RenderIndexed(*mPreviewMeshBuffer,
                                  *mMaterial,
                                  *mCamera,
                                  glm::toMat4(mProperties.ModelRotationQuat));
    }

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
        longestDist = std::max(dist, longestDist);
      }
    }

    mProperties.MaxDistance =
      longestDist / (tanf(0.5F * mCamera->GetProperties().Fov * DEG_2_RAD));
    mCamera->GetProperties().Transform.GetPosition() = {
      0, 0, 1.3F * mProperties.MaxDistance * mProperties.Distance
    };
    mCamera->GetProperties().NearPlane = 0.1F;
    mCamera->GetProperties().NearPlane =
      1.3F * mProperties.MaxDistance + longestDist;
  }
}