#pragma once
#include "pch.h"
#include "Core/Scene/IScene.h"
#include "Core/Scene/Properties/ISceneProperties.h"
#include "Core/Scene/Entity/Entity.h"
#include "Core/Scene/Components/SceneComponents.h"

namespace Dwarf
{
  /// @brief Class that represents a Dwarf Engine scene.
  class Scene : public IScene
  {
  public:
    Scene(const nlohmann::json&             serializedSceneGraph,
          std::unique_ptr<ISceneProperties> properties);
    ~Scene();

    /// @brief Retrieves the asset reference of the scene.
    /// @return The asset reference.
    entt::registry&
    GetRegistry() override;

    /// @brief Retrieves the root entity of the scene. This is the parent of all
    /// entities in the scene.
    /// @return The root entity.
    Entity&
    GetRootEntity() override;

    /// @brief Retrieves the settings of the scene.
    /// @return The settings.
    ISceneProperties&
    GetProperties() override;

    /// @brief Creates a new entity with a given name.
    /// @param name Name of the entity.
    /// @return The created entity instance.
    Entity
    CreateEntity(const std::string& name) override;

    /// @brief Deletes an entity from the scene.
    /// @param entity Entity to delete.
    void
    DeleteEntity(const Entity& entity) override;

    /// @brief Serializes the scene to a JSON object.
    /// @return The JSON object.
    nlohmann::json
    Serialize() const override;

  private:
    /// @brief The registry that holds all entities and components.
    entt::registry m_Registry = entt::registry();

    /// @brief The root entity in the scene graph.
    Entity m_RootEntity;

    /// @brief The settings of the scene.
    std::unique_ptr<ISceneProperties> m_Properties;

    /// @brief Because of dependency cycle
    // friend class Entity;

    /// @brief Creates the root entity.
    Entity
    CreateRootEntity();

    /// @brief Creates a new entity with a given name a UID.
    /// @param uid UID to use with the entity.
    /// @param name Name of the entity.
    /// @return The created entity instance.
    Entity
    CreateEntityWithUID(const UUID& uid, const std::string& name);

    /// @brief Returns the recursive model matrix of a transform.
    /// @param transform A transform component instance.
    /// @return 4x4 model matrix composition of a transform and its full parent
    /// chain.
    glm::mat4
    GetFullModelMatrix(TransformComponent& transform);

    /// @brief Deserializes the scene from a JSON object.
    void
    Deserialize(const nlohmann::json& serializedSceneGraph);

    Entity
    DeserializeEntity(const nlohmann::json& serializedEntity);
  };
}