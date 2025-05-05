#include "pch.hpp"

#include "Core/Rendering/AntiAliasingTypes.hpp"
#include "Core/Rendering/ExposureTypes.hpp"
#include "Core/Rendering/Framebuffer/IFramebuffer.hpp"
#include "Core/Rendering/Material/IMaterial.hpp"
#include "Core/Scene/Components/MeshRendererComponentHandle.hpp"
#include "Core/Scene/Components/SceneComponents.hpp"
#include "Core/Scene/Components/TransformComponentHandle.hpp"
#include "IRenderingPipeline.hpp"
#include "RenderingPipeline.hpp"

namespace Dwarf
{
  RenderingPipeline::RenderingPipeline(
    std::shared_ptr<IRendererApi>    rendererApi,
    std::shared_ptr<IShaderRegistry> shaderRegistry,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                                 shaderSourceCollectionFactory,
    std::shared_ptr<IMeshFactory>                meshFactory,
    std::shared_ptr<IMeshBufferFactory>          meshBufferFactory,
    std::shared_ptr<ILoadedScene>                loadedScene,
    const std::shared_ptr<IFramebufferFactory>&  framebufferFactory,
    const std::shared_ptr<IMaterialFactory>&     materialFactory,
    const std::shared_ptr<IDrawCallListFactory>& drawCallListFactory,
    const std::shared_ptr<IDrawCallWorkerFactory>& drawCallWorkerFactory,
    const std::shared_ptr<IPingPongBufferFactory>& pingPongBufferFactory)
    : mRendererApi(std::move(rendererApi))
    , mLoadedScene(std::move(loadedScene))
    , mShaderRegistry(std::move(shaderRegistry))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
    , mMeshFactory(std::move(meshFactory))
    , mMeshBufferFactory(std::move(meshBufferFactory))
    , mDrawCallList(drawCallListFactory->Create())
    , mDrawCallWorker(drawCallWorkerFactory->Create(mDrawCallList))
  {
    mLoadedScene->RegisterLoadedSceneObserver(this);
    mRendererApi->SetClearColor(glm::vec4(0.065F, 0.07F, 0.085F, 1.0F));

    mIdMaterial = materialFactory->CreateMaterial(
      mShaderSourceCollectionFactory->CreateIdShaderSourceCollection());
    mIdMaterial->GetShader()->Compile();
    mGridShader = mShaderRegistry->GetOrCreate(
      mShaderSourceCollectionFactory->CreateGridShaderSourceCollection());

    SetupRenderFramebuffer(framebufferFactory);

    SetupPingPongBuffers(pingPongBufferFactory);

    SetupPresentationFramebuffer(framebufferFactory);

    SetupIdFramebuffer(framebufferFactory);

    mDrawCallWorker->Invalidate();
  }

  RenderingPipeline::~RenderingPipeline()
  {
    mDrawCallList->Clear();
  }

  void
  RenderingPipeline::SetupRenderFramebuffer(
    const std::shared_ptr<IFramebufferFactory>& framebufferFactory)
  {
    FramebufferSpecification spec = GetSpecification();
    mRenderFramebuffer = framebufferFactory->Create(spec);
  }

  void
  RenderingPipeline::SetupPingPongBuffers(
    const std::shared_ptr<IPingPongBufferFactory>& pingPongBufferFactory)
  {
    FramebufferSpecification hdrPingPongSpec;
    FramebufferSpecification ldrPingPongSpec;
    hdrPingPongSpec.Attachments.Attachments.emplace_back(
      FramebufferTextureFormat::RGBA16F);
    hdrPingPongSpec.Attachments.Attachments.emplace_back(
      FramebufferTextureFormat::DEPTH);
    ldrPingPongSpec.Attachments.Attachments.emplace_back(
      FramebufferTextureFormat::SRGBA8);

    mHdrPingPong = pingPongBufferFactory->Create(hdrPingPongSpec);
    mLdrPingPong = pingPongBufferFactory->Create(ldrPingPongSpec);
  }

  void
  RenderingPipeline::SetupPresentationFramebuffer(
    const std::shared_ptr<IFramebufferFactory>& framebufferFactory)
  {
    FramebufferSpecification presentationSpec;
    presentationSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::SRGBA8 }
    };
    mPresentationBuffer = framebufferFactory->Create(presentationSpec);
  }

  void
  RenderingPipeline::SetupIdFramebuffer(
    const std::shared_ptr<IFramebufferFactory>& framebufferFactory)
  {
    FramebufferSpecification idSpec;
    idSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RED_INTEGER },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    mIdBuffer = framebufferFactory->Create(idSpec);
  }

  void
  RenderingPipeline::RenderScene(ICamera& camera, GridSettingsData gridSettings)
  {
    // ==================== Scene Rendering ====================

    mRenderFramebuffer->Bind();
    mRenderFramebuffer->SetDrawBuffer(0);
    mRendererApi->SetViewport(0,
                              0,
                              mRenderFramebuffer->GetSpecification().Width,
                              mRenderFramebuffer->GetSpecification().Height);
    mRendererApi->SetClearColor(glm::vec4(0.065F, 0.07F, 0.085F, 1.0F));
    mRendererApi->Clear();

    // TODO: Render skybox
    {
      std::lock_guard<std::mutex> lock(mDrawCallList->GetMutex());
      for (auto& drawCall : mDrawCallList->GetDrawCalls())
      {
        if (drawCall->GetMeshBuffer() != nullptr)
        {
          mRendererApi->RenderIndexed(
            drawCall->GetMeshBuffer(),
            drawCall->GetMaterialAsset().GetMaterial(),
            camera,
            drawCall->GetTransform().GetMatrix());
        }
      }
    }

    mRenderFramebuffer->Unbind();

    // ==================== Resolve MSAA ====================

    mRendererApi->Blit(*mRenderFramebuffer,
                       *mHdrPingPong->GetWriteFramebuffer().lock(),
                       0,
                       0,
                       mRenderFramebuffer->GetSpecification().Width,
                       mRenderFramebuffer->GetSpecification().Height);
    mRendererApi->BlitDepth(*mRenderFramebuffer,
                            *mHdrPingPong->GetWriteFramebuffer().lock(),
                            mRenderFramebuffer->GetSpecification().Width,
                            mRenderFramebuffer->GetSpecification().Height);
    mHdrPingPong->Swap();

    // ==================== HDR Post Processing ====================

    // TODO

    // ==================== Tonemapping ====================

    if (mTonemapShader && mRendererApi && mHdrPingPong &&
        mHdrPingPong->GetReadFramebuffer().lock() && mLdrPingPong &&
        mLdrPingPong->GetWriteFramebuffer().lock())
    {
      mTonemapShader->SetParameter("hdrTexture",
                                   mHdrPingPong->GetReadTexture());
      mRendererApi->CustomBlit(*mHdrPingPong->GetReadFramebuffer().lock(),
                               *mLdrPingPong->GetWriteFramebuffer().lock(),
                               0,
                               0,
                               *mTonemapShader,
                               true);
      mLdrPingPong->Swap();
    }

    // ==================== LDR Post Processing ====================

    if (gridSettings.RenderGrid)
    {
      mGridShader->SetParameter("uSceneDepth",
                                mHdrPingPong->GetReadFramebuffer()
                                  .lock()
                                  ->GetDepthAttachment()
                                  .value());
      mGridShader->SetParameter("uSceneColor", mLdrPingPong->GetReadTexture());
      mGridShader->SetParameter("uGridHeight", gridSettings.GridYOffset);
      mGridShader->SetParameter("uOpacity", gridSettings.GridOpacity);
      mGridShader->SetParameter(
        "uInverseViewProjection",
        glm::inverse(camera.GetProjectionMatrix() * camera.GetViewMatrix()));
      mGridShader->SetParameter("uInverseView",
                                glm::inverse(camera.GetViewMatrix()));
      mGridShader->SetParameter("uCameraPosition",
                                camera.GetProperties().Transform.Position);

      mRendererApi->ApplyPostProcess(*mLdrPingPong, *mGridShader, true);
      mLdrPingPong->Swap();
    }

    // ==================== Presentation ====================

    mRendererApi->Blit(*mLdrPingPong->GetReadFramebuffer().lock(),
                       *mPresentationBuffer,
                       0,
                       0,
                       mRenderFramebuffer->GetSpecification().Width,
                       mRenderFramebuffer->GetSpecification().Height);
  }

  auto
  RenderingPipeline::GetSpecification() const -> FramebufferSpecification
  {
    FramebufferSpecification fbSpec;
    fbSpec.Attachments = FramebufferAttachmentSpecification{
      FramebufferTextureSpecification{ FramebufferTextureFormat::RGBA16F },
      FramebufferTextureSpecification{ FramebufferTextureFormat::DEPTH }
    };
    fbSpec.Samples = 1;

    return fbSpec;
  }

  void
  RenderingPipeline::RenderIds(IScene& scene, ICamera& camera)
  {
    mIdBuffer->Bind();
    mRendererApi->Clear(0);

    mRendererApi->SetViewport(0,
                              0,
                              mIdBuffer->GetSpecification().Width,
                              mIdBuffer->GetSpecification().Height);

    for (auto view = scene.GetRegistry()
                       .view<TransformComponent, MeshRendererComponent>();
         auto [entity, transform, component] : view.each())
    {
      MeshRendererComponentHandle meshRenderer(scene.GetRegistry(), entity);
      if (meshRenderer.GetModelAsset() && !meshRenderer.GetIsHidden())
      {
        if (!meshRenderer.GetIdMeshBuffer())
        {
          auto& model =
            dynamic_cast<ModelAsset&>(meshRenderer.GetModelAsset()->GetAsset());
          std::shared_ptr<IMesh> mergedMesh =
            mMeshFactory->MergeMeshes(model.Meshes());
          meshRenderer.SetIdMeshBuffer(
            std::move(mMeshBufferFactory->Create(mergedMesh)));
        }

        glm::mat4 modelMatrix =
          TransformComponentHandle(scene.GetRegistry(), entity).GetMatrix();
        auto entityId = (uint32_t)entity;
        mIdMaterial->GetShaderParameters()->SetParameter("objectId", entityId);
        mRendererApi->RenderIndexed(
          meshRenderer.GetIdMeshBuffer(), *mIdMaterial, camera, modelMatrix);
      }
    }
    mIdBuffer->Unbind();
  }

  auto
  RenderingPipeline::GetResolution() -> glm::ivec2
  {
    return { mRenderFramebuffer->GetSpecification().Width,
             mRenderFramebuffer->GetSpecification().Height };
  }

  void
  RenderingPipeline::SetResolution(glm::ivec2 resolution)
  {
    mRenderFramebuffer->Resize(resolution.x, resolution.y);
    mIdBuffer->Resize(resolution.x, resolution.y);
    mPresentationBuffer->Resize(resolution.x, resolution.y);
    mHdrPingPong->Resize(resolution);
    mLdrPingPong->Resize(resolution);
  }

  auto
  RenderingPipeline::GetMsaaSamples() -> int32_t
  {
    return mRenderFramebuffer->GetSpecification().Samples;
  }

  void
  RenderingPipeline::SetMsaaSamples(int32_t samples)
  {
    mRenderFramebuffer->SetSamples(samples);
  }

  auto
  RenderingPipeline::GetMaxMsaaSamples() -> int32_t
  {
    return mRendererApi->GetMaxSamples();
  }

  auto
  RenderingPipeline::ReadPixelId(glm::ivec2 position) -> uint32_t
  {
    return mIdBuffer->ReadPixel(0, position.x, position.y);
  }

  auto
  RenderingPipeline::GetPresentationBufferId() -> uintptr_t
  {
    return mPresentationBuffer->GetColorAttachment().value()->GetTextureID();
  }

  void
  RenderingPipeline::SetExposure(float exposure)
  {
    if (mTonemapShader)
    {
      mTonemapShader->SetParameter("exposure", exposure);
    }
  }

  [[nodiscard]] auto
  RenderingPipeline::GetDrawCallCount() const -> uint32_t
  {
    return mDrawCallList->GetStats().DrawCallCount.load();
  }

  [[nodiscard]] auto
  RenderingPipeline::GetVertexCount() const -> uint32_t
  {
    return mDrawCallList->GetStats().VertexCount.load();
  }

  [[nodiscard]] auto
  RenderingPipeline::GetTriangleCount() const -> uint32_t
  {
    return mDrawCallList->GetStats().TriangleCount.load();
  }

  void
  RenderingPipeline::SetTonemapType(TonemapType type)
  {
    switch (type)
    {
      using enum TonemapType;
      case Reinhard:
        {
          mTonemapShader = mShaderRegistry->GetOrCreate(
            mShaderSourceCollectionFactory
              ->CreateReinhardTonemapShaderSourceCollection());
          break;
        }
      case Agx:
        {
          mTonemapShader = mShaderRegistry->GetOrCreate(
            mShaderSourceCollectionFactory
              ->CreateAgxTonemapShaderSourceCollection());
          break;
        }
      case Aces:
        {
          mTonemapShader = mShaderRegistry->GetOrCreate(
            mShaderSourceCollectionFactory
              ->CreateAcesTonemapShaderSourceCollection());
          break;
        }
      case Uncharted2:
        {
          mTonemapShader = mShaderRegistry->GetOrCreate(
            mShaderSourceCollectionFactory
              ->CreateUncharted2TonemapShaderSourceCollection());
          break;
        }
    }

    OnExposureSettingsChanged();
  }

  void
  RenderingPipeline::OnAntiAliasingSettingsChanged()
  {
    switch (mLoadedScene->GetScene()
              .GetProperties()
              .GetSettings()
              .GetAntiAliasingSettings()
              .GetAntiAliasingMethod())
    {
      using enum AntiAliasingMethod;
      case MSAA:
        SetMsaaSamples(mLoadedScene->GetScene()
                         .GetProperties()
                         .GetSettings()
                         .GetAntiAliasingSettings()
                         .GetSamples());
        break;
      case FXAA:
      case TAA:
      case None: SetMsaaSamples(1); break;
    }
  }

  void
  RenderingPipeline::OnExposureSettingsChanged()
  {
    if (mLoadedScene->GetScene()
          .GetProperties()
          .GetSettings()
          .GetExposureSettings()
          .GetExposureType() == ExposureType::Manual)
    {
      SetExposure(mLoadedScene->GetScene()
                    .GetProperties()
                    .GetSettings()
                    .GetExposureSettings()
                    .GetExposure());
    }
  }

  void
  RenderingPipeline::OnTonemapChanged()
  {
    SetTonemapType(
      mLoadedScene->GetScene().GetProperties().GetSettings().GetToneMapType());
  }

  void
  RenderingPipeline::OnBloomSettingsChanged()
  {
  }

  void
  RenderingPipeline::OnSceneLoad()
  {
    if (mLoadedScene->HasLoadedScene())
    {
      mLoadedScene->GetScene()
        .GetProperties()
        .GetSettings()
        .RegisterSceneSettingsObserver(this);
      OnTonemapChanged();
      OnExposureSettingsChanged();
      OnAntiAliasingSettingsChanged();
    }
  }

  void
  RenderingPipeline::OnSceneUnload()
  {
    if (mLoadedScene->HasLoadedScene())
    {
      mLoadedScene->GetScene()
        .GetProperties()
        .GetSettings()
        .UnregisterSceneSettingsObserver(this);
    }
  }
}