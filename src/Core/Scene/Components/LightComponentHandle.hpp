#pragma once

#include "Core/GenericComponents.hpp"
#include "Core/Scene/Components/SceneComponents.hpp"

namespace Dwarf
{
  class LightComponentHandle
  {
  private:
    entt::registry& mRegistry;
    entt::entity    mEntity;

  public:
    LightComponentHandle(entt::registry& reg, entt::entity ent)
      : mRegistry(reg)
      , mEntity(ent)
    {
    }

    /// @brief Returns the type of light this component represents.
    /// @return The light type.
    [[nodiscard]] auto
    GetType() const -> LightType
    {
      return mRegistry.get<LightComponent>(mEntity).Type;
    }

    void
    SetType(LightType lightType)
    {
      mRegistry.patch<LightComponent>(
        mEntity,
        [lightType](LightComponent& component) mutable
        { component.Type = lightType; });
    }

    /// @brief Returns the color of the light.
    /// @return The color as a 3D vector.
    auto
    GetColor() -> glm::vec3
    {
      return mRegistry.get<LightComponent>(mEntity).Color;
    }

    void
    SetColor(glm::vec3 lightColor)
    {
      mRegistry.patch<LightComponent>(
        mEntity,
        [lightColor](LightComponent& component) mutable
        { component.Color = lightColor; });
    }

    /// @brief Returns the attenuation of the light.
    /// @return The attenuation as a float.
    auto
    GetAttenuation() -> float
    {
      return mRegistry.get<LightComponent>(mEntity).Attenuation;
    }

    void
    SetAttenuation(float attenuation)
    {
      mRegistry.patch<LightComponent>(
        mEntity,
        [attenuation](LightComponent& component) mutable
        { component.Attenuation = attenuation; });
    }

    /// @brief Returns the radius of the point light.
    /// @return The radius as a float.
    auto
    GetRadius() -> float
    {
      return mRegistry.get<LightComponent>(mEntity).Radius;
    }

    void
    SetRadius(float radius)
    {
      mRegistry.patch<LightComponent>(
        mEntity,
        [radius](LightComponent& component) mutable
        { component.Radius = radius; });
    }

    /// @brief Returns the opening angle of the spot light.
    /// @return The opening angle as a float.
    auto
    GetOpeningAngle() -> float&
    {
      return mRegistry.get<LightComponent>(mEntity).OpeningAngle;
    }

    void
    SetOpeningAngle(float openingAngle)
    {
      mRegistry.patch<LightComponent>(
        mEntity,
        [openingAngle](LightComponent& component) mutable
        { component.OpeningAngle = openingAngle; });
    }
  };
}