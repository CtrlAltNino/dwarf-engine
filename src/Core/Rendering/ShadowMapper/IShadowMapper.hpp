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
    std::vector<PointLightShadow>  PointLightShadows;
    >
  }
};

class IShadowMapper
{
public:
  virtual ~IShadowMapper() = default;

  virtual void
  Update(LightData& lightData) = 0;

  [[nodiscard]] virtual auto
  GetFrameData() const -> const ShadowFrameData = 0;

  virtual void
  Bind() const = 0;
};
}