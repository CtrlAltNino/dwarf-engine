#pragma once
#include "../../utilities/dpch.h"

#include"../data structures/Entity.h"
#include"EntityProvider.h"
#include"../components/Components.h"

//#include<glad/glad.h>

#include"Object.h"
#include"Camera.h"

/// @brief Class that represents a Dwarf Engine scene.
class Scene : public EntityProvider {
public:
	/// @brief The root entity in the scene graph.
	Entity rootEntity = CreateRootEntity();

	/// @brief Entities that have been selected by the user.
	std::vector<Entity> selectedEntities;
	// ========== Constructors ==========
	
	Scene(const char* sceneName);
	~Scene();
	
	// ========== Getters ==========

	const char* getSceneName();
	
	// ========== Scene Functions ==========

	/// @brief Creates the root entity.
	/// @return The created entity instance.
	Entity CreateRootEntity();
	
	/// @brief Creates a new entity with a given name.
	/// @param name Name of the entity.
	/// @return The created entity instance.
	Entity CreateEntity(const std::string& name = std::string());

	/// @brief Creates a new entity with a given name a UID.
	/// @param uid UID to use with the entity.
	/// @param name Name of the entity.
	/// @return The created entity instance.
	Entity CreateEntityWithUID(UID uid, const std::string& name);

	/// @brief Draws the scene from the POV of a given camera.
	/// @param camera Camera instance.
	void drawScene(Camera camera);

	/// @brief Sets the selection to a single entity.
	/// @param entity Entity instance.
	void selectEntity(Entity entity);

	/// @brief Adds an entity to the selection.
	/// @param entity Entity instance.
	void addEntityToSelection(Entity entity);

	/// @brief Removes an entity from the selection.
	/// @param entity Entity instance.
	void removeEntityFromSelection(Entity entity);

	/// @brief Checks if an entity is selected.
	/// @param entity Entity instance.
	/// @return True if selected, false if not.
	bool isEntitySelected(Entity entity);

	/// @brief Clears the selection list.
	void clearSelection();
private:
	/// @brief Name of the opened scene.
	const char* sceneName;

	/// @brief Because of dependency cycle
	friend class Entity;

	/// @brief Returns the tree index of a given entity. Used for sorting based on their graph positions.
	/// @param entity Entity instance.
	/// @return The full tree index.
	std::string GetTreeIndex(Entity entity);

	/// @brief Returns the recursive model matrix of a transform.
	/// @param transform A transform component instance.
	/// @return 4x4 model matrix composition of a transform and its full parent chain.
	glm::mat4 GetFullModelMatrix(TransformComponent transform);
};