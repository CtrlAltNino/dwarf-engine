#include "Core/Scene/Components/SceneComponents.h"
#include "Core/Scene/Scene.h"
#include <Core/Asset/Database/AssetDatabase.h>

namespace Dwarf
{
  Scene::Scene(nlohmann::json                    serializedSceneGraph,
               std::shared_ptr<ISceneProperties> properties)
    : m_Properties(properties)
  {
    CreateRootEntity();
    Deserialize(serializedSceneGraph);
  }

  Scene::~Scene()
  {
    // TODO: Clear registry
  }

  void
  Scene::Deserialize(const nlohmann::json& serializedSceneGraph)
  {
    // SceneSettings settings;

    // settings.fogSettings.fogColor = { serializedScene["fog"]["color"]["r"],
    //                                   serializedScene["fog"]["color"]["g"],
    //                                   serializedScene["fog"]["color"]["b"] };
    // settings.fogSettings.fogStart = serializedScene["fog"]["fogStart"];
    // settings.fogSettings.fogEnd = serializedScene["fog"]["fogEnd"];
    // settings.fogSettings.type = (FogType)serializedScene["fog"]["type"];

    // settings.globalLightSettings.color = {
    //   serializedScene["globalLight"]["color"]["r"],
    //   serializedScene["globalLight"]["color"]["g"],
    //   serializedScene["globalLight"]["color"]["b"]
    // };
    // settings.globalLightSettings.intensity =
    //   serializedScene["globalLight"]["intensity"];

    // if (serializedScene.contains("skyboxMaterial") &&
    //     serializedScene["skyboxMaterial"] != "null")
    // {
    //   settings.skyboxMaterial =
    //     std::make_shared<UID>((uint64_t)serializedScene["skyboxMaterial"]);
    // }

    // std::shared_ptr<Scene> deserializedScene =
    //   std::make_shared<Scene>(path, settings);

    for (auto const& element : serializedSceneGraph["hierarchy"])
    {
      Entity newEntity = DeserializeEntity(element);
      newEntity.SetParent(GetRootEntity()->GetHandle());
    }
  }

  Entity
  Scene::DeserializeEntity(nlohmann::json serializedEntity)
  {
    Entity newEntity = CreateEntityWithUID(
      UID((uint64_t)serializedEntity["guid"]), serializedEntity["name"]);

    TransformComponent& tc = newEntity.GetComponent<TransformComponent>();
    tc.position = { serializedEntity["transformComponent"]["position"]["x"],
                    serializedEntity["transformComponent"]["position"]["y"],
                    serializedEntity["transformComponent"]["position"]["z"] };
    tc.rotation = { serializedEntity["transformComponent"]["rotation"]["x"],
                    serializedEntity["transformComponent"]["rotation"]["y"],
                    serializedEntity["transformComponent"]["rotation"]["z"] };
    tc.scale = { serializedEntity["transformComponent"]["scale"]["x"],
                 serializedEntity["transformComponent"]["scale"]["y"],
                 serializedEntity["transformComponent"]["scale"]["z"] };

    if (serializedEntity.contains("lightComponent"))
    {
      LightComponent& lightComponent = newEntity.AddComponent<LightComponent>();
      lightComponent.type =
        (LightComponent::LIGHT_TYPE)serializedEntity["lightComponent"]["type"];
      lightComponent.lightColor = {
        serializedEntity["lightComponent"]["lightColor"]["r"],
        serializedEntity["lightComponent"]["lightColor"]["g"],
        serializedEntity["lightComponent"]["lightColor"]["b"]
      };

      lightComponent.attenuation =
        serializedEntity["lightComponent"]["attenuation"];

      lightComponent.radius = serializedEntity["lightComponent"]["radius"];

      lightComponent.openingAngle =
        serializedEntity["lightComponent"]["openingAngle"];
    }

    if (serializedEntity.contains("meshRendererComponent"))
    {
      MeshRendererComponent& meshRendererComponent =
        newEntity.AddComponent<MeshRendererComponent>();

      meshRendererComponent.canCastShadow =
        (bool)serializedEntity["meshRendererComponent"]["canCastShadows"];

      if (serializedEntity["meshRendererComponent"].contains("mesh") &&
          serializedEntity["meshRendererComponent"]["mesh"] != "null")
      {
        meshRendererComponent.meshAsset = std::make_shared<UID>(
          (uint64_t)serializedEntity["meshRendererComponent"]["mesh"]);
      }

      if (serializedEntity["meshRendererComponent"].contains("materials"))
      {
        for (auto const& element :
             serializedEntity["meshRendererComponent"]["materials"])
        {
          meshRendererComponent.materialAssets.push_back(
            (uint64_t)element == -1 ? nullptr
                                    : std::make_shared<UID>((uint64_t)element));
        }
      }
    }

    for (auto const& element : serializedEntity["children"])
    {
      Entity childEntity = DeserializeEntity(element);
      childEntity.SetParent(newEntity.GetHandle());
    }

    return newEntity;
  }

  void
  Scene::CreateRootEntity()
  {
    m_RootEntity = std::make_shared<Entity>(m_Registry->create(), m_Registry);
    m_RootEntity->AddComponent<IDComponent>(UID());
    m_RootEntity->AddComponent<TransformComponent>();
    m_RootEntity->AddComponent<NameComponent>("Root");
  }

  Entity
  Scene::CreateEntity(const std::string& name)
  {
    return CreateEntityWithUID(UID(), name);
  }

  Entity
  Scene::CreateEntityWithUID(UID uid, const std::string& name)
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

  std::shared_ptr<ISceneProperties>
  Scene::GetProperties()
  {
    return m_Properties;
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