#pragma once

#include "Core/Base.h"
#include "Core/Scene/Entity.h"
#include "Core/Scene/SceneComponents.h"
#include "Core/Scene/Camera.h"
#include "Core/Rendering/Framebuffer.h"

namespace Dwarf
{
  enum class FogType
  {
    LINEAR,
    EXPONENTIAL
  };

  struct FogSettings
  {
    glm::vec3 fogColor = { 0.3f, 0.3f, 0.3f };
    float     fogStart = 20.0f;
    float     fogEnd = 50.0f;
    FogType   type = FogType::LINEAR;
  };

  struct GlobalLightSettings
  {
    glm::vec3 color = { 0.8f, 0.6f, 0.6f };
    float     intensity = 0.2f;
  };

  struct SceneSettings
  {
    std::shared_ptr<UID> skyboxMaterial;
    FogSettings          fogSettings;
    GlobalLightSettings  globalLightSettings;
  };

  /// @brief Class that represents a Dwarf Engine scene.
  class Scene
  {
  public:
    // ========== Constructors ==========
    Scene(std::filesystem::path const& path, SceneSettings const& settings);
    ~Scene();

    // ========== Getters ==========

    std::string
    GetName() const;
    std::filesystem::path
    GetPath() const;
    std::shared_ptr<entt::registry>
    GetRegistry() const;
    std::shared_ptr<Entity>
    GetRootEntity() const;
    SceneSettings
    GetSettings() const;

    // ========== Setters ==========
    void
    SetPath(std::filesystem::path const& path);

    // ========== Scene Functions ==========

    /// @brief Creates a new entity with a given name.
    /// @param name Name of the entity.
    /// @return The created entity instance.
    Entity
    CreateEntity(const std::string& name = std::string()) const;

    /// @brief Creates a new entity with a given name a UID.
    /// @param uid UID to use with the entity.
    /// @param name Name of the entity.
    /// @return The created entity instance.
    Entity
    CreateEntityWithUID(UID uid, const std::string& name) const;

    void
    DeleteEntity(Entity entity);

  private:
    /// @brief Name of the opened scene.
    std::string m_Name;

    std::filesystem::path m_Path;

    std::shared_ptr<entt::registry> m_Registry =
      std::make_shared<entt::registry>();

    SceneSettings m_Settings;

    /// @brief The root entity in the scene graph.
    std::shared_ptr<Entity> m_RootEntity;

    /// @brief Because of dependency cycle
    friend class Entity;

    /// @brief Creates the root entity.
    /// @return The created entity instance.
    void
    CreateRootEntity();

    /// @brief Returns the recursive model matrix of a transform.
    /// @param transform A transform component instance.
    /// @return 4x4 model matrix composition of a transform and its full parent
    /// chain.
    glm::mat4
    GetFullModelMatrix(TransformComponent const& transform) const;
  };
}