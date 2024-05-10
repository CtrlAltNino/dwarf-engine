
#include "Core/Scene/SceneComponents.h"
#include "Core/Scene/Scene.h"
#include <Core/Asset/AssetDatabase.h>
#include <Platform/OpenGL/OpenGLMesh.h>

namespace Dwarf
{

  // ========== Constructors ==========

  Scene::Scene(std::filesystem::path const& path, SceneSettings const& settings)
    : m_Name(path.empty() ? "New Scene" : path.stem().string())
    , m_Path(path)
    , m_Settings(settings)
  {
    CreateRootEntity();
  }

  Scene::~Scene() = default;

  // ========== Getters ==========

  std::string
  Scene::GetName() const
  {
    return m_Name;
  }

  std::filesystem::path
  Scene::GetPath() const
  {
    return m_Path;
  }

  // ========== Scene Functions ==========

  void
  Scene::CreateRootEntity()
  {
    m_RootEntity = std::make_shared<Entity>(m_Registry->create(), m_Registry);
    m_RootEntity->AddComponent<IDComponent>(UID());
    m_RootEntity->AddComponent<TransformComponent>();
    m_RootEntity->AddComponent<NameComponent>("Root");
  }

  Entity
  Scene::CreateEntity(const std::string& name) const
  {
    return CreateEntityWithUID(UID(), name);
  }

  Entity
  Scene::CreateEntityWithUID(UID uid, const std::string& name) const
  {
    Entity entity(m_Registry->create(), m_Registry);
    entity.AddComponent<IDComponent>(uid);
    entity.AddComponent<TransformComponent>();
    auto& nameComp = entity.AddComponent<NameComponent>(name);
    nameComp.Name = name.empty() ? "Entity" : name;

    entity.SetParent(m_RootEntity->GetHandle());

    return entity;
  }

  std::shared_ptr<entt::registry>
  Scene::GetRegistry() const
  {
    return m_Registry;
  }

  std::shared_ptr<Entity>
  Scene::GetRootEntity() const
  {
    return m_RootEntity;
  }

  SceneSettings
  Scene::GetSettings() const
  {
    return m_Settings;
  }

  void
  Scene::SetPath(std::filesystem::path const& path)
  {
    m_Path = path;
    m_Name = path.stem().string();

    // TODO: UPDATE WINDOW TITLE BAR
  }

  glm::mat4
  Scene::GetFullModelMatrix(TransformComponent const& transform) const
  {
    auto                   parentMat = glm::mat4(1.0f);
    std::vector<glm::mat4> matriceStack;

    matriceStack.push_back(transform.getModelMatrix());

    if (entt::entity cursor = transform.parent; cursor != entt::null)
    {
      Entity cur(cursor, m_Registry);
      matriceStack.push_back(
        cur.GetComponent<TransformComponent>().getModelMatrix());
    }

    for (auto it = matriceStack.rbegin(); it < matriceStack.rend(); it++)
    {
      parentMat *= *it;
    }

    return parentMat;
  }

  void
  Scene::DeleteEntity(Entity entity)
  {
    if (m_Registry->valid(entity.GetHandle()))
    {
      if (!entity.GetChildren()->empty())
      {
        for (entt::entity child : *(entity.GetChildren()))
        {
          DeleteEntity(Entity(child, m_Registry));
        }
      }

      Entity(entity.GetParent(), m_Registry).RemoveChild(entity.GetHandle());
      m_Registry->destroy(entity.GetHandle());
    }
  }
}