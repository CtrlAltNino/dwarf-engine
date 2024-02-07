#pragma once

#include "Core/Base.h"
#include "Core/Scene/Entity.h"
#include "Core/Scene/SceneComponents.h"
#include "Core/Scene/Camera.h"
#include "Core/Rendering/Framebuffer.h"

namespace Dwarf
{
	enum FogType
	{
		LINEAR,
		EXPONENTIAL
	};

	struct FogSettings
	{
		glm::vec3 fogColor = {0.3, 0.3, 0.3};
		float fogStart = 20.0f;
		float fogEnd = 50.0f;
		FogType type = FogType::LINEAR;
	};

	struct GlobalLightSettings
	{
		glm::vec3 color = {0.8, 0.6, 0.6};
		float intensity = 0.2;
	};

	struct SceneSettings
	{
		Ref<UID> skyboxMaterial;
		FogSettings fogSettings;
		GlobalLightSettings globalLightSettings;
	};

	/// @brief Class that represents a Dwarf Engine scene.
	class Scene
	{
	public:
		// ========== Constructors ==========
		Scene(std::filesystem::path path, SceneSettings settings);
		~Scene();

		// ========== Getters ==========

		std::string GetName();
		std::filesystem::path GetPath();
		// std::vector<Entity>* GetSelectedEntities();
		Ref<entt::registry> GetRegistry();
		Ref<Entity> GetRootEntity();
		SceneSettings GetSettings();

		// ========== Setters ==========
		void SetPath(std::filesystem::path path);

		// ========== Scene Functions ==========

		/// @brief Creates a new entity with a given name.
		/// @param name Name of the entity.
		/// @return The created entity instance.
		Entity CreateEntity(const std::string &name = std::string());

		/// @brief Creates a new entity with a given name a UID.
		/// @param uid UID to use with the entity.
		/// @param name Name of the entity.
		/// @return The created entity instance.
		Entity CreateEntityWithUID(UID uid, const std::string &name);

		void DeleteEntity(Entity entity);

	private:
		/// @brief Name of the opened scene.
		std::string m_Name;

		std::filesystem::path m_Path;

		Ref<entt::registry> m_Registry;

		SceneSettings m_Settings;

		/// @brief The root entity in the scene graph.
		Ref<Entity> m_RootEntity;

		/// @brief Because of dependency cycle
		friend class Entity;

		/// @brief Creates the root entity.
		/// @return The created entity instance.
		void CreateRootEntity();

		/// @brief Returns the tree index of a given entity. Used for sorting based on their graph positions.
		/// @param entity Entity instance.
		/// @return The full tree index.
		std::string GetTreeIndex(Entity entity);

		/// @brief Returns the recursive model matrix of a transform.
		/// @param transform A transform component instance.
		/// @return 4x4 model matrix composition of a transform and its full parent chain.
		glm::mat4 GetFullModelMatrix(TransformComponent transform);
	};
}