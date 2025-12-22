#pragma once

#include "Core/Rendering/Texture/ITexture.hpp"
#include "Core/Scene/Components/LightComponentHandle.hpp"
namespace Dwarf
{
  struct PointLightShadow
  {
    LightComponentHandle lightEntity;

    // ShadowMap shadowMap;
    std::shared_ptr<ITexture> ShadowMap;

    glm::mat4 lightSpaceMatrix;

    bool dirty = true;
  };

  struct DirectionalShadow
  {
    LightComponentHandle lightEntity;

    // ShadowMap shadowMap;
    std::shared_ptr<ITexture> ShadowMap;

    glm::mat4 lightSpaceMatrix;

    bool dirty = true;
  };

  struct ShadowFrameData
  {
    std::optional<DirectionalShadow> DirectionalShadow;
    std::vector<PointLightShadow>    PointLightShadows;
  };

  class IShadowMapper
  {
  public:
    virtual ~IShadowMapper() = default;

    virtual void
    Update() = 0;

    [[nodiscard]] virtual auto
    GetFrameData() const -> const ShadowFrameData = 0;
  };
}