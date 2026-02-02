#pragma once

#include "Core/Scene/Components/LightComponentHandle.hpp"
#include "Core/Scene/Components/TransformComponentHandle.hpp"
namespace Dwarf
{
  struct DirectionalLightData
  {
    glm::vec3 Direction;
    float     Intensity;

    glm::vec3 Color;
    int       CastsShadows;

    int       ShadowIndex;
    glm::vec3 _padding; // pad to 16-byte alignment

    auto
    operator==(const DirectionalLightData& other) const -> bool
    {
      return Direction == other.Direction && Intensity == other.Intensity &&
             Color == other.Color && CastsShadows == other.CastsShadows &&
             ShadowIndex == other.ShadowIndex;
    }
  };

  struct PointLightData
  {
    glm::vec3 Position;
    float     Intensity;

    glm::vec3 Color;
    int       CastsShadows;

    float     Radius;
    int       ShadowIndex;
    glm::vec2 _padding; // pad to 16-byte alignment

    auto
    operator==(const PointLightData& other) const -> bool
    {
      return Position == other.Position && Intensity == other.Intensity &&
             Color == other.Color && CastsShadows == other.CastsShadows &&
             Radius == other.Radius && ShadowIndex == other.ShadowIndex;
    }
  };

  struct LightData
  {
    std::vector<DirectionalLightData> DirectionalLights;
    std::vector<PointLightData>       PointLights;

    auto
    operator==(const LightData& other) const -> bool
    {
      return DirectionalLights == other.DirectionalLights &&
             PointLights == other.PointLights;
    }
  };

  struct LightInfo
  {
    LightComponentHandle     Handle;
    TransformComponentHandle Transform;
    size_t                   ShadowIndex;
  };

  class ILightSystem
  {
  public:
    virtual ~ILightSystem() = default;

    virtual void
    UpdateRegistry() = 0;

    virtual void
    UpdateLightData() = 0;

    [[nodiscard]] virtual auto
    GetLightData() -> LightData& = 0;

    [[nodiscard]] virtual auto
    GetLightRegistry() -> std::unordered_map<UUID, LightInfo>& = 0;

    virtual void
    Bind() const = 0;
  };
}