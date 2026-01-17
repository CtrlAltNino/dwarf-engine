#include "Core/Rendering/LightTypes.hpp"
#include "Core/Scene/Components/LightComponent.hpp"
#include "Core/Scene/Components/TransformComponent.hpp"
#include "Core/Scene/Components/TransformComponentHandle.hpp"
#include "ILightSystem.hpp"
#include "pch.hpp"
#include <glm/ext/vector_float3.hpp>

#include "LightSystem.hpp"

namespace Dwarf
{
  LightSystem::LightSystem(std::shared_ptr<IDwarfLogger> logger,
                           std::shared_ptr<ILoadedScene> loadedScene)
    : mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
  {
    mLoadedScene->RegisterLoadedSceneObserver(this);
    glGenBuffers(1, &directionalLightSSBO);
    glGenBuffers(1, &pointLightSSBO);
  }

  LightSystem::~LightSystem()
  {
    mLoadedScene->UnregisterLoadedSceneObserver(this);
  }

  auto
  GetForwardDirection(const TransformComponent& transform) -> glm::vec3
  {
    // Convert degrees → radians
    glm::vec3 radians = glm::radians(transform.GetEulerAngles());

    glm::mat4 rotation =
      glm::rotate(glm::mat4(1.0F), radians.y, glm::vec3(0, 1, 0)) * // yaw
      glm::rotate(glm::mat4(1.0F), radians.x, glm::vec3(1, 0, 0)) * // pitch
      glm::rotate(glm::mat4(1.0F), radians.z, glm::vec3(0, 0, 1));  // roll

    // Forward = -Z
    glm::vec3 forward =
      glm::normalize(glm::vec3(rotation * glm::vec4(0, 0, -1, 0)));

    return forward;
  }

  void
  LightSystem::Update()
  {
    // LightComponent view
    if (mLoadedScene->HasLoadedScene())
    {

      mTempLightData = LightData();
      auto view = mLoadedScene->GetScene()
                    .GetRegistry()
                    .view<IDComponent, LightComponent, TransformComponent>();
      for (auto entity : view)
      {
        auto& light = view.get<LightComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);

        if (!light.Enabled)
        {
          continue;
        }

        switch (light.Type)
        {
          using enum LightType;
          case Directional:
            {
              DirectionalLightData lightData{};
              lightData.CastsShadows = light.CastsShadows;
              lightData.Color = light.Color;
              auto fullMatrix =
                mLoadedScene->GetScene().GetFullModelMatrix(transform);
              auto      rotation = glm::mat3(fullMatrix);
              glm::vec3 direction = rotation * glm::vec3(0.0f, 0.0f, -1.0f);

              lightData.Direction = glm::normalize(direction);
              lightData.Intensity = light.Attenuation;

              mTempLightData.DirectionalLights.push_back(lightData);
              break;
            }
          case PointLight:
            {
              PointLightData lightData{};
              lightData.CastsShadows = light.CastsShadows;
              lightData.Color = light.Color;
              auto fullMatrix =
                mLoadedScene->GetScene().GetFullModelMatrix(transform);
              lightData.Position = glm::vec3(fullMatrix[3]);
              lightData.Intensity = light.Attenuation;
              lightData.Radius = light.Radius;

              mTempLightData.PointLights.push_back(lightData);
            }
            break;
          case SpotLight: break;
        }
      }

      if (mTempLightData != mLightData)
      {
        mLightData = mTempLightData;

        Upload();
      }
    }
  }

  auto
  LightSystem::GetLightData() -> LightData&
  {
    return mLightData;
  }

  void
  LightSystem::Upload()
  {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, directionalLightSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 mLightData.DirectionalLights.size() *
                   sizeof(DirectionalLightData),
                 mLightData.DirectionalLights.data(),
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, pointLightSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 mLightData.PointLights.size() * sizeof(PointLightData),
                 mLightData.PointLights.data(),
                 GL_DYNAMIC_DRAW);
  }

  void
  LightSystem::Bind() const
  {
    constexpr GLuint directionalLightBinding = 3;
    constexpr GLuint pointLightBinding = 4;

    glBindBufferBase(
      GL_SHADER_STORAGE_BUFFER, directionalLightBinding, directionalLightSSBO);

    glBindBufferBase(
      GL_SHADER_STORAGE_BUFFER, pointLightBinding, pointLightSSBO);
  }

  void
  LightSystem::OnSceneLoad()
  {
  }

  void
  LightSystem::OnSceneUnload()
  {
  }
}