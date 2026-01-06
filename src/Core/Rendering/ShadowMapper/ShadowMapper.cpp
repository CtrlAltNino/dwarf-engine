#include "pch.hpp"

#include "ShadowMapper.hpp"

#include <utility>

namespace Dwarf
{
  ShadowMapper::ShadowMapper(std::shared_ptr<ILoadedScene> loadedScene,
                             std::shared_ptr<IFramebuffer> framebufferFactory)
    : mLoadedScene(std::move(loadedScene))
    , mFramebufferFactory(std::move(framebufferFactory))
  {
  }

  void
  ShadowMapper::Update(LightData& lightData)
  {
    // Reset per-frame shadow data
    m_DirectionalShadows.clear();
    m_PointShadows.clear();

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
        DirectionalShadow shadow{};
        BuildDirectionalShadow(directionalLight, shadow);

        m_DirectionalShadows.push_back(std::move(shadow));
      }
    }

    for (auto& pointLight : lightData.PointLights)
    {
      if (!pointLight.CastsShadows)
      {
        pointLight.ShadowIndex = -1;
        continue;
      }

      // Assign shadow index
      pointLight.ShadowIndex = nextShadowIndex++;

      // Create shadow data
      PointShadow shadow{};
      BuildPointShadow(pointLight, shadow);

      m_PointShadows.push_back(std::move(shadow));
    }
  }

  [[nodiscard]] auto
  ShadowMapper::GetFrameData() const -> const ShadowFrameData
  {
  }

  void
  ShadowMapper::OnShadowSettingsChanged()
  {
  }
}