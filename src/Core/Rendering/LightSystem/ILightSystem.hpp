#pragma once

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

  class ILightSystem
  {
  public:
    virtual ~ILightSystem() = default;

    virtual void
    Update() = 0;

    [[nodiscard]] virtual auto
    GetLightData() -> LightData& = 0;

    virtual void
    Bind() const = 0;
  };
}