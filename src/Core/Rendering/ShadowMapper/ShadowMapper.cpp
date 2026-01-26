#include "pch.hpp"

#include "Core/Rendering/Framebuffer/IFramebuffer.hpp"
#include "ShadowMapper.hpp"
#include <glm/ext/vector_float3.hpp>
#include <utility>

namespace Dwarf
{
  ShadowMapper::ShadowMapper(
    std::shared_ptr<ILoadedScene>        loadedScene,
    std::shared_ptr<IFramebufferFactory> framebufferFactory,
    std::shared_ptr<ITextureFactory>     textureFactory,
    std::shared_ptr<IShaderRegistry>     shaderRegistry)
    : mLoadedScene(std::move(loadedScene))
    , mFramebufferFactory(std::move(framebufferFactory))
    , mTextureFactory(std::move(textureFactory))
    , mShaderRegistry(std::move(shaderRegistry))
  {
    mLoadedScene->RegisterLoadedSceneObserver(this);
    if (mLoadedScene->HasLoadedScene())
    {
      OnSceneLoad();
    }
  }

  /**
   * @brief mDepthAttachment = std::move(mTextureFactory->Empty(
      TextureType::TEXTURE_2D,
      TextureFormat::DEPTH,
      TextureDataType::FLOAT,
      glm::ivec2(mSpecification.Width, mSpecification.Height),
      mSpecification.Samples));
   *
   * @param lightData
   */

  void
  ShadowMapper::Update(LightData& lightData)
  {
    // Reset per-frame shadow data
    // mShadowFrameData.DirectionalShadows.clear();
    // mShadowFrameData.PointLightShadows.clear();

    // Stop here when shadows are disabled
    if (!mLoadedScene->GetScene()
           .GetProperties()
           .GetSettings()
           .GetShadowSettings()
           .GetEnabled())
    {
      return;
    }

    // Iterate over directional lights
    int nextShadowIndex = 0;
    for (auto& directionalLight : lightData.DirectionalLights)
    {
      if (!directionalLight.CastsShadows)
      {
        directionalLight.ShadowIndex = -1;
        continue;
      }

      // Assign shadow index
      directionalLight.ShadowIndex = nextShadowIndex++;

      // Create shadow data
      DirectionalShadow shadow = ProcessDirectionalLight(directionalLight);

      // Add it
      mShadowFrameData.DirectionalShadows.push_back(std::move(shadow));
    }

    // for (auto& pointLight : lightData.PointLights)
    // {
    //   if (!pointLight.CastsShadows)
    //   {
    //     pointLight.ShadowIndex = -1;
    //     continue;
    //   }

    //   // Assign shadow index
    //   pointLight.ShadowIndex = nextShadowIndex++;

    //   // Create shadow data
    //   PointLightShadow shadow{};
    //   BuildPointShadow(pointLight, shadow);

    //   m_PointShadows.push_back(std::move(shadow));
    // }
  }

  auto
  ShadowMapper::ProcessDirectionalLight(
    DirectionalLightData& directionalLightData) -> DirectionalShadow
  {
    DirectionalShadow shadow;

    shadow.LightSpaceMatrix =
      BuildDirectionalLightVP(directionalLightData.Direction);
    // BuildDirectionalShadow(directionalLight, shadow);

    return shadow;
  }

  auto
  ShadowMapper::BuildDirectionalLightVP(const glm::vec3& dir) -> glm::mat4
  {
    glm::vec3 lightDir = glm::normalize(dir);

    // glm::vec3 center = mLoadedScene->GetSceneBounds().Center;
    auto center = glm::vec3(0);
    // float     radius = mLoadedScene->GetSceneBounds().Radius;
    float radius = 50;

    glm::vec3 lightPos = center - lightDir * radius * 2.0f;

    glm::mat4 view = glm::lookAt(lightPos, center, glm::vec3(0, 1, 0));

    glm::mat4 proj =
      glm::ortho(-radius, radius, -radius, radius, 0.1f, radius * 4.0f);

    return proj * view;
  }

  [[nodiscard]] auto
  ShadowMapper::GetFrameData() const -> const ShadowFrameData
  {
    return mShadowFrameData;
  }

  void
  ShadowMapper::Bind() const
  {
  }

  void
  ShadowMapper::OnShadowSettingsChanged()
  {
    // TODO: React to shadow settings being changed
    if (!mDepthTexture || mDepthTexture->GetSpecification().Height !=
                            ToResolution(mLoadedScene->GetScene()
                                           .GetProperties()
                                           .GetSettings()
                                           .GetShadowSettings()
                                           .GetResolution()))
    {
      FramebufferSpecification fbSpec;
      fbSpec.Height = ToResolution(mLoadedScene->GetScene()
                                     .GetProperties()
                                     .GetSettings()
                                     .GetShadowSettings()
                                     .GetResolution());
      fbSpec.Width = ToResolution(mLoadedScene->GetScene()
                                    .GetProperties()
                                    .GetSettings()
                                    .GetShadowSettings()
                                    .GetResolution());
      fbSpec.Samples = 1;

      FramebufferTextureSpecification depthSpec;
      depthSpec.TextureFormat = FramebufferTextureFormat::DEPTH;
      spec.Attachments.Attachments.push_back(depthSpec);
      mDepthTexture = mFramebufferFactory->Create(spec);
    }
  }

  void
  ShadowMapper::OnSceneLoad()
  {
    if (mLoadedScene->HasLoadedScene())
    {
      mLoadedScene->GetScene()
        .GetProperties()
        .GetSettings()
        .RegisterSceneSettingsObserver(this);
      OnShadowSettingsChanged();
    }
  }

  void
  ShadowMapper::OnSceneUnload()
  {
    if (mLoadedScene->HasLoadedScene())
    {
      mLoadedScene->GetScene()
        .GetProperties()
        .GetSettings()
        .UnregisterSceneSettingsObserver(this);
    }
  }

  void
  ShadowMapper::BindForShadowPass(std::shared_ptr<ITexture> depthTexture)
  {
    mShadowRenderFB->AttachDepthTexture(depthTexture);

#ifdef DEBUG
    assert(mFramebuffer->IsComplete());
#endif

    mShadowRenderFB->Bind();
  }
}