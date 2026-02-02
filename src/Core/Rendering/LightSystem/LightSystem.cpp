#include "pch.hpp"

#include "LightSystem.hpp"

#include "Core/GenericComponents.hpp"
#include "Core/Rendering/LightTypes.hpp"
#include "Core/Scene/Components/LightComponent.hpp"
#include "Core/Scene/Components/LightComponentHandle.hpp"
#include "Core/Scene/Components/TransformComponent.hpp"
#include "Core/Scene/Components/TransformComponentHandle.hpp"
#include "ILightSystem.hpp"
#include <glm/ext/vector_float3.hpp>

namespace Dwarf
{
  LightSystem::LightSystem(std::shared_ptr<IDwarfLogger> logger,
                           std::shared_ptr<ILoadedScene> loadedScene)
    : mLogger(std::move(logger))
    , mLoadedScene(std::move(loadedScene))
  {
    mLoadedScene->RegisterLoadedSceneObserver(this);
    glGenBuffers(1, &mDirectionalLightSSBO);
    glGenBuffers(1, &mPointLightSSBO);
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
  LightSystem::UpdateRegistry()
  {
    mLightRegistry.clear();

    // LightComponent view
    if (mLoadedScene->HasLoadedScene())
    {
      mLightData = LightData();
      auto view = mLoadedScene->GetScene()
                    .GetRegistry()
                    .view<IDComponent, LightComponent, TransformComponent>();
      for (auto entity : view)
      {
        auto& light = view.get<LightComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);
        auto& id = view.get<IDComponent>(entity);

        if (!light.Enabled)
        {
          continue;
        }

        mLightRegistry.try_emplace(
          id.getId(),
          LightComponentHandle(&mLoadedScene->GetScene().GetRegistry(), entity),
          TransformComponentHandle(&mLoadedScene->GetScene().GetRegistry(),
                                   entity),
          0);
      }
    }
  }

  void
  LightSystem::UpdateLightData()
  {
    for (auto& lightInfo : mLightRegistry)
    {
      switch (lightInfo.second.Handle.GetType())
      {
        using enum LightType;
        case Directional:
          {
            DirectionalLightData lightData{};
            lightData.CastsShadows =
              static_cast<int>(lightInfo.second.Handle.GetCastsShadows());
            lightData.Color = lightInfo.second.Handle.GetColor();
            auto fullMatrix = mLoadedScene->GetScene().GetFullModelMatrix(
              mLoadedScene->GetScene().GetRegistry().get<TransformComponent>(
                lightInfo.second.Transform.GetHandle()));
            auto      rotation = glm::mat3(fullMatrix);
            glm::vec3 direction = rotation * glm::vec3(0.0F, 0.0F, -1.0F);

            lightData.Direction = glm::normalize(direction);
            lightData.Intensity = lightInfo.second.Handle.GetAttenuation();

            mLightData.DirectionalLights.push_back(lightData);
            break;
          }
        case PointLight:
          {
            PointLightData lightData{};
            lightData.CastsShadows =
              static_cast<int>(lightInfo.second.Handle.GetCastsShadows());
            lightData.Color = lightInfo.second.Handle.GetColor();
            auto fullMatrix = mLoadedScene->GetScene().GetFullModelMatrix(
              mLoadedScene->GetScene().GetRegistry().get<TransformComponent>(
                lightInfo.second.Transform.GetHandle()));
            lightData.Position = glm::vec3(fullMatrix[3]);
            lightData.Intensity = lightInfo.second.Handle.GetAttenuation();
            lightData.Radius = lightInfo.second.Handle.GetRadius();

            mLightData.PointLights.push_back(lightData);
          }
          break;
        case SpotLight: break;
      }
    }

    Upload();
  }

  auto
  LightSystem::GetLightData() -> LightData&
  {
    return mLightData;
  }

  auto
  LightSystem::GetLightRegistry() -> std::unordered_map<UUID, LightInfo>&
  {
    return mLightRegistry;
  }

  void
  LightSystem::Upload()
  {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mDirectionalLightSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER,
                 mLightData.DirectionalLights.size() *
                   sizeof(DirectionalLightData),
                 mLightData.DirectionalLights.data(),
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mPointLightSSBO);
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
      GL_SHADER_STORAGE_BUFFER, directionalLightBinding, mDirectionalLightSSBO);

    glBindBufferBase(
      GL_SHADER_STORAGE_BUFFER, pointLightBinding, mPointLightSSBO);
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