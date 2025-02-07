#pragma once

#include "Core/Scene/Properties/ISceneProperties.h"
#include "Utilities/ISerializable.h"
#include "Core/Scene/Entity/Entity.h"
#include <entt/entity/fwd.hpp>
namespace Dwarf
{
  /// @brief Interface that represents a Dwarf Engine scene.
  class IScene : public ISerializable
  {
  public:
    /// @brief Destructor.
    virtual ~IScene() = default;

    /// @brief Retrieves the registry of the scene. This is the ECS registry
    /// that holds all entities and components.
    /// @return The registry.
    virtual entt::registry&
    GetRegistry() = 0;

    /// @brief Retrieves the root entity of the scene. This is the parent of
    /// all entities in the scene.
    /// @return The root entity.
    virtual Entity&
    GetRootEntity() = 0;

    /// @brief Retrieves the properties of the scene.
    /// @return The properties.
    virtual ISceneProperties&
    GetProperties() = 0;

    /// @brief Creates a new entity with a given name.
    /// @param name Name of the entity.
    /// @return The created entity instance.
    virtual Entity
    CreateEntity(const std::string& name) = 0;

    /// @brief Deletes an entity from the scene.
    /// @param entity Entity to delete.
    virtual void
    DeleteEntity(const Entity& entity) = 0;
  };
}