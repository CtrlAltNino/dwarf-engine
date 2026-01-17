#include "pch.hpp"

#include "ShadowMapper.hpp"

#include <glm/ext/vector_float3.hpp>
#include <utility>

namespace Dwarf
{
  ShadowMapper::ShadowMapper(
    std::shared_ptr<ILoadedScene>        loadedScene,
    std::shared_ptr<IFramebufferFactory> framebufferFactory,
    std::shared_ptr<IShaderRegistry>     shaderRegistry)
    : mLoadedScene(std::move(loadedScene))
    , mFramebufferFactory(std::move(framebufferFactory))
    , mShaderRegistry(std::move(shaderRegistry))
  {
    mLoadedScene->RegisterLoadedSceneObserver(this);
    if (mLoadedScene->HasLoadedScene())
    {
      OnSceneLoad();
    }
  }

  void
  ShadowMapper::Update(LightData& lightData)
  {
    // Reset per-frame shadow data
    // m_DirectionalShadows.clear();
    // m_PointShadows.clear();
    mShadowFrameData.DirectionalShadows.clear();
    mShadowFrameData.PointLightShadows.clear();

    int nextShadowIndex = 0;
    for (auto& directionalLight : lightData.DirectionalLights)
    {
      if (directionalLight.CastsShadows)
      {
        if (!directionalLight.CastsShadows)
        {
          directionalLight.ShadowIndex = -1;
          continue;
        }

        // Assign shadow index
        directionalLight.ShadowIndex = nextShadowIndex++;

        // Create shadow data
        // DirectionalShadow shadow{};
        // BuildDirectionalShadow(directionalLight, shadow);

        // mShadowFrameData.DirectionalShadows.push_back(std::move(shadow));
      }
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
}