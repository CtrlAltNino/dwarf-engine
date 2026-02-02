#pragma once

#include "Core/Rendering/LightSystem/ILightSystem.hpp"
#include "Core/Rendering/Texture/ITexture.hpp"
#include "Core/UUID.hpp"

namespace Dwarf
{
  struct PointShadow
  {
    std::shared_ptr<ITexture> ShadowTexture;
    glm::mat4                 LightSpaceMatrix;
  };

  struct DirectionalShadow
  {
    std::shared_ptr<ITexture> ShadowTexture;
    glm::mat4                 LightSpaceMatrix;
  };

  struct ShadowFrameData
  {
    std::vector<DirectionalShadow> DirectionalShadows;
    std::vector<PointShadow>       PointLightShadows;
  };

  class IShadowMapper
  {
  public:
    virtual ~IShadowMapper() = default;

    virtual void
    Update(std::unordered_map<UUID, LightInfo>& lightRegistry) = 0;

    [[nodiscard]] virtual auto
    GetFrameData() const -> const ShadowFrameData = 0;

    virtual void
    Bind() const = 0;
  };
}