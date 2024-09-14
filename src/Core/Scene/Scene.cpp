#include "Core/Scene/Components/SceneComponents.h"
#include "Core/Scene/Scene.h"
#include <Core/Asset/Database/AssetDatabase.h>
#include <string>

namespace Dwarf
{
  Scene::Scene(const nlohmann::json&             serializedSceneGraph,
               std::unique_ptr<ISceneProperties> properties)
    : m_Properties(std::move(properties))
    , m_RootEntity(CreateRootEntity())
  {
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
    //     std::make_shared<UUID>((uint64_t)serializedScene["skyboxMaterial"]);
    // }

    // std::shared_ptr<Scene> deserializedScene =
    //   std::make_shared<Scene>(path, settings);
    if (serializedSceneGraph.contains("hierarchy"))
    {
      for (auto const& element : serializedSceneGraph["hierarchy"])
      {
        Entity newEntity = DeserializeEntity(element);
        newEntity.SetParent(m_RootEntity.GetHandle());
      }
    }
  }

  Entity
  Scene::DeserializeEntity(const nlohmann::json& serializedEntity)
  {
    Entity newEntity =
      CreateEntityWithUID(UUID(serializedEntity["guid"].get<std::string>()),
                          serializedEntity["name"]);

    /*TransformComponent& tc = newEntity.GetComponent<TransformComponent>();
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
        meshRendererComponent.ModelAsset() = UUID(
          serializedEntity["meshRendererComponent"]["mesh"].get<std::string>());
      }

      if (serializedEntity["meshRendererComponent"].contains("materials"))
      {
        for (const auto& element :
             serializedEntity["meshRendererComponent"]["materials"])
        {
          meshRendererComponent.MaterialAssets().push_back(
            UUID(element.get<std::string>()));
        }
      }
    }

    for (auto const& element : serializedEntity["children"])
    {
      Entity childEntity = DeserializeEntity(element);
      childEntity.SetParent(newEntity.GetHandle());
    }*/

    return newEntity;
  }

  Entity
  Scene::CreateRootEntity()
  {
    Entity m_RootEntity(m_Registry.create(), m_Registry);
    m_RootEntity.AddComponent<IDComponent>(UUID());
    m_RootEntity.AddComponent<TransformComponent>();
    m_RootEntity.AddComponent<NameComponent>("Root");
    return m_RootEntity;
  }

  Entity
  Scene::CreateEntity(const std::string& name)
  {
    return CreateEntityWithUID(UUID(), name);
  }

  Entity
  Scene::CreateEntityWithUID(const UUID& uid, const std::string& name)
  {
    Entity entity(m_Registry.create(), m_Registry);
    entity.AddComponent<IDComponent>(uid);
    entity.AddComponent<TransformComponent>();
    auto& nameComp = entity.AddComponent<NameComponent>(name);
    nameComp.Name = name.empty() ? "Entity" : name;

    entity.SetParent(m_RootEntity.GetHandle());

    return entity;
  }

  entt::registry&
  Scene::GetRegistry()
  {
    return m_Registry;
  }

  Entity&
  Scene::GetRootEntity()
  {
    return m_RootEntity;
  }

  ISceneProperties&
  Scene::GetProperties()
  {
    return *m_Properties;
  }

  glm::mat4
  Scene::GetFullModelMatrix(TransformComponent& transform)
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
  Scene::DeleteEntity(const Entity& entity)
  {
    if (m_Registry.valid(entity.GetHandle()))
    {
      if (!entity.GetChildren().empty())
      {
        for (entt::entity child : entity.GetChildren())
        {
          DeleteEntity(Entity(child, m_Registry));
        }
      }

      Entity(entity.GetParent(), m_Registry).RemoveChild(entity.GetHandle());
      m_Registry.destroy(entity.GetHandle());
    }
  }

  nlohmann::json
  Scene::Serialize() const
  {
    nlohmann::json serializedSceneGraph;

    // TODO: Serialize Scene
    // serializedSceneGraph["hierarchy"] = nlohmann::json::array();

    // for (entt::entity entity : m_Registry->view<IDComponent>())
    // {
    //   Entity e(entity, m_Registry);
    //   if (e.GetParent() == m_RootEntity->GetHandle())
    //   {
    //     serializedSceneGraph["hierarchy"].push_back(e.Serialize());
    //   }
    // }

    return serializedSceneGraph;
  }
}