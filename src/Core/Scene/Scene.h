#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Scene/Components/SceneComponents.h"
#include "Core/Scene/Entity/Entity.h"
#include "Core/Scene/IScene.h"
#include "Core/Scene/Properties/ISceneProperties.h"
#include <boost/serialization/strong_typedef.hpp>
#include <memory>

namespace Dwarf
{
  BOOST_STRONG_TYPEDEF(nlohmann::json, SerializedGraph)
  /// @brief Class that represents a Dwarf Engine scene.
  class Scene : public IScene
  {
  private:
    /// @brief The registry that holds all entities and components.
    entt::registry                  mRegistry;
    std::shared_ptr<IAssetDatabase> mAssetDatabase;

    /// @brief The root entity in the scene graph.
    Entity mRootEntity;

    /// @brief The settings of the scene.
    std::unique_ptr<ISceneProperties> mProperties;

    /// @brief Because of dependency cycle
    // friend class Entity;

    /// @brief Creates the root entity.
    auto
    CreateRootEntity() -> Entity;

    /// @brief Creates a new entity with a given name a UID.
    /// @param uid UID to use with the entity.
    /// @param name Name of the entity.
    /// @return The created entity instance.
    auto
    CreateEntityWithUID(const UUID& uid, const std::string& name) -> Entity;

    /// @brief Returns the recursive model matrix of a transform.
    /// @param transform A transform component instance.
    /// @return 4x4 model matrix composition of a transform and its full parent
    /// chain.
    auto
    GetFullModelMatrix(TransformComponent& transform) -> glm::mat4 override;

    /// @brief Deserializes the scene from a JSON object.
    void
    Deserialize(const nlohmann::json& serializedSceneGraph);

    auto
    DeserializeEntity(const nlohmann::json& serializedEntity) -> Entity;

  public:
    Scene(std::unique_ptr<ISceneProperties> properties,
          std::shared_ptr<IAssetDatabase>   assetDatabase);

    Scene(const SerializedGraph&            serializedScene,
          std::unique_ptr<ISceneProperties> properties,
          std::shared_ptr<IAssetDatabase>   assetDatabase);

    ~Scene() override;

    /// @brief Retrieves the asset reference of the scene.
    /// @return The asset reference.
    auto
    GetRegistry() -> entt::registry& override;

    /// @brief Retrieves the root entity of the scene. This is the parent of all
    /// entities in the scene.
    /// @return The root entity.
    auto
    GetRootEntity() -> Entity& override;

    /// @brief Retrieves the settings of the scene.
    /// @return The settings.
    auto
    GetProperties() -> ISceneProperties& override;

    /// @brief Creates a new entity with a given name.
    /// @param name Name of the entity.
    /// @return The created entity instance.
    auto
    CreateEntity(const std::string& name) -> Entity override;

    /// @brief Deletes an entity from the scene.
    /// @param entity Entity to delete.
    void
    DeleteEntity(const Entity& entity) override;

    /// @brief Serializes the scene to a JSON object.
    /// @return The JSON object.
    auto
    Serialize() -> nlohmann::json override;
  };
}