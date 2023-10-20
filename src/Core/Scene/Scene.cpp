#include "dpch.h"
#include "Core/Scene/SceneComponents.h"
#include "Core/Scene/Scene.h"

namespace Dwarf
{

	// ========== Constructors ==========

	Scene::Scene(std::filesystem::path path, SceneSettings settings)
		: m_Name(path.empty() ? "New Scene" : path.stem().string()),
		  m_Path(path), m_Registry(CreateRef<entt::registry>(entt::registry())),
		  m_Settings(settings)
	{
		CreateRootEntity();
	}

	Scene::~Scene() {}

	// ========== Getters ==========

	std::string Scene::GetName()
	{
		return m_Name;
	}

	std::filesystem::path Scene::GetPath()
	{
		return m_Path;
	}

	// ========== Scene Functions ==========

	void Scene::CreateRootEntity()
	{
		m_RootEntity = CreateRef<Entity>(m_Registry->create(), m_Registry);
		m_RootEntity->AddComponent<IDComponent>(UID());
		m_RootEntity->AddComponent<TransformComponent>();
		auto &name = m_RootEntity->AddComponent<NameComponent>("Root");
	}

	Entity Scene::CreateEntity(const std::string &name)
	{
		return CreateEntityWithUID(UID(), name);
	}

	Entity Scene::CreateEntityWithUID(UID uid, const std::string &name)
	{
		Entity entity(m_Registry->create(), m_Registry);
		entity.AddComponent<IDComponent>(uid);
		entity.AddComponent<TransformComponent>();
		auto &nameComp = entity.AddComponent<NameComponent>(name);
		nameComp.Name = name.empty() ? "Entity" : name;

		entity.SetParent(m_RootEntity->GetHandle());

		return entity;
	}

	/*void Scene::drawScene(Camera camera) {

		glm::mat4x4 viewMatrix = camera.GetViewMatrix();
		glm::mat4x4 projectionMatrix = camera.GetProjectionMatrix();

		auto view = m_Registry.view<TransformComponent, MeshRendererComponent>();

		// Rendering all opague meshes

		// use forward iterators and get only the components of interest
		for(auto entity: view) {
			auto &transform = view.get<TransformComponent>(entity);
			auto &renderer = view.get<MeshRendererComponent>(entity);

			for(auto mesh : renderer.meshes){
				Material mat = renderer.materials.at(mesh.materialIndex);
				if(!mat.isTransparent){
					mat.bind();
					//mat.UpdateMVP(transform.getModelMatrix(), viewMatrix, projectionMatrix);
					mat.UpdateMVP(GetFullModelMatrix(transform), viewMatrix, projectionMatrix);
					mat.UpdateUniforms();

					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);

					mesh.bind();
					glDrawElements(GL_TRIANGLES, mesh.indices2.size(), GL_UNSIGNED_INT, 0);
					mesh.unbind();

					glDisable(GL_CULL_FACE);
				}
			}
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		for(auto entity: view) {
			auto &transform = view.get<TransformComponent>(entity);
			auto &renderer = view.get<MeshRendererComponent>(entity);

			for(auto mesh : renderer.meshes){
				Material mat = renderer.materials.at(mesh.materialIndex);
				if(mat.isTransparent){
					mat.bind();
					//mat.UpdateMVP(transform.getModelMatrix(), viewMatrix, projectionMatrix);
					mat.UpdateMVP(GetFullModelMatrix(transform), viewMatrix, projectionMatrix);
					mat.UpdateUniforms();

					mesh.bind();
					glDrawElements(GL_TRIANGLES, mesh.indices2.size(), GL_UNSIGNED_INT, 0);
					mesh.unbind();
				}
			}
		}

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
	}*/

	Ref<entt::registry> Scene::GetRegistry()
	{
		return m_Registry;
	}

	Ref<Entity> Scene::GetRootEntity()
	{
		return m_RootEntity;
	}

	SceneSettings Scene::GetSettings()
	{
		return m_Settings;
	}

	void Scene::SetPath(std::filesystem::path path)
	{
		m_Path = path;
		m_Name = path.stem().string();

		// TODO UPDATE WINDOW TITLE BAR
	}

	glm::mat4 Scene::GetFullModelMatrix(TransformComponent transform)
	{
		glm::mat4 parentMat = glm::mat4(1.0f);
		std::vector<glm::mat4> matriceStack;

		matriceStack.push_back(transform.getModelMatrix());

		entt::entity cursor = transform.parent;

		if (cursor != entt::null)
		{
			Entity cur = Entity(cursor, m_Registry);
			matriceStack.push_back(cur.GetComponent<TransformComponent>().getModelMatrix());

			cursor = cur.GetParent();
		}

		for (auto it = matriceStack.rbegin(); it < matriceStack.rend(); it++)
		{
			parentMat *= *it;
		}

		return parentMat;
	}
}