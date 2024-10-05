#include "Core/Scene/Components/SceneComponents.h"
#include "Core/Scene/Scene.h"
#include <Core/Asset/Database/AssetDatabase.h>
#include <entt/entity/fwd.hpp>
#include <string>

namespace Dwarf
{
  Scene::Scene(std::unique_ptr<ISceneProperties> properties,
               std::shared_ptr<IAssetDatabase>   assetDatabase)
    : m_Registry(entt::registry())
    , m_Properties(std::move(properties))
    , m_RootEntity(CreateRootEntity())
    , m_AssetDatabase(assetDatabase)
  {
  }

  Scene::Scene(SerializedGraph                   serializedScene,
               std::unique_ptr<ISceneProperties> properties,
               std::shared_ptr<IAssetDatabase>   assetDatabase)
    : m_Registry(entt::registry())
    , m_Properties(std::move(properties))
    , m_RootEntity(CreateRootEntity())
    , m_AssetDatabase(assetDatabase)
  {
    Deserialize(serializedScene.t);
  }

  Scene::~Scene()
  {
    // TODO: Clear registry
    // m_Registry.clear();
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
    std::cout << "serialized graph:\n"
              << serializedSceneGraph.dump(2) << std::endl;
    for (auto const& element : serializedSceneGraph)
    {
      std::cout << "Deserializing entity" << std::endl;
      Entity newEntity = DeserializeEntity(element);
      newEntity.SetParent(m_RootEntity.GetHandle());
    }
  }

  Entity
  Scene::DeserializeEntity(const nlohmann::json& serializedEntity)
  {
    Entity newEntity =
      CreateEntityWithUID(UUID(serializedEntity["guid"].get<std::string>()),
                          serializedEntity["name"]);

    TransformComponent& tc = newEntity.GetComponent<TransformComponent>();
    tc.GetPosition() = {
      serializedEntity["TransformComponent"]["Position"]["x"],
      serializedEntity["TransformComponent"]["Position"]["y"],
      serializedEntity["TransformComponent"]["Position"]["z"]
    };
    tc.GetEulerAngles() = {
      serializedEntity["TransformComponent"]["Rotation"]["x"],
      serializedEntity["TransformComponent"]["Rotation"]["y"],
      serializedEntity["TransformComponent"]["Rotation"]["z"]
    };
    tc.GetScale() = { serializedEntity["TransformComponent"]["Scale"]["x"],
                      serializedEntity["TransformComponent"]["Scale"]["y"],
                      serializedEntity["TransformComponent"]["Scale"]["z"] };

    if (serializedEntity.contains("LightComponent"))
    {
      newEntity.AddComponent<LightComponent>(
        serializedEntity["LightComponent"]);
    }

    if (serializedEntity.contains("MeshRendererComponent"))
    {
      std::unique_ptr<IAssetReference> modelAsset = m_AssetDatabase->Retrieve(
        UUID(serializedEntity["MeshRendererComponent"]["Model"]
               .get<std::string>()));

      std::vector<std::unique_ptr<IAssetReference>> materialAssets;

      for (auto const& element :
           serializedEntity["MeshRendererComponent"]["Materials"])
      {
        materialAssets.push_back(
          element.get<std::string>() != "0"
            ? m_AssetDatabase->Retrieve(UUID(element.get<std::string>()))
            : nullptr);
      }

      MeshRendererComponent& comp =
        newEntity.AddComponent<MeshRendererComponent>(
          std::move(modelAsset), std::move(materialAssets));

      if (serializedEntity["MeshRendererComponent"].contains("Hidden"))
      {
        comp.IsHidden() = serializedEntity["MeshRendererComponent"]["Hidden"];
      }
    }

    if (serializedEntity.contains("Children"))
    {
      for (auto const& element : serializedEntity["Children"])
      {
        Entity childEntity = DeserializeEntity(element);
        childEntity.SetParent(newEntity.GetHandle());
      }
    }

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

    matriceStack.push_back(transform.GetModelMatrix());

    if (entt::entity cursor = transform.GetParent(); cursor != entt::null)
    {
      Entity cur(cursor, m_Registry);
      matriceStack.push_back(
        cur.GetComponent<TransformComponent>().GetModelMatrix());
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
  Scene::Serialize()
  {
    nlohmann::json serializedScene;

    serializedScene["Settings"] = m_Properties->Serialize();

    // TODO: Serialize Scene
    serializedScene["Graph"] = nlohmann::json::array();

    for (entt::entity entity : m_Registry.view<IDComponent>())
    {
      Entity e(entity, m_Registry);
      if (e.GetParent() == m_RootEntity.GetHandle())
      {
        serializedScene["Graph"].push_back(e.Serialize());
      }
    }

    /*for (entt::entity entity : m_Registry.view<IDComponent>())
    {
      Entity entity(entity, scene->GetRegistry());

      serializedArray[entityCount]["guid"] =
        (uint64_t)(*entity.GetComponent<IDComponent>().ID);
      serializedArray[entityCount]["name"] =
        entity.GetComponent<NameComponent>().Name;

      TransformComponent tc = entity.GetComponent<TransformComponent>();
      serializedArray[entityCount]["transformComponent"]["position"]["x"] =
        tc.getPosition().x;
      serializedArray[entityCount]["transformComponent"]["position"]["y"] =
        tc.getPosition().y;
      serializedArray[entityCount]["transformComponent"]["position"]["z"] =
        tc.getPosition().z;

      serializedArray[entityCount]["transformComponent"]["rotation"]["x"] =
        tc.getEulerAngles().x;
      serializedArray[entityCount]["transformComponent"]["rotation"]["y"] =
        tc.getEulerAngles().y;
      serializedArray[entityCount]["transformComponent"]["rotation"]["z"] =
        tc.getEulerAngles().z;

      serializedArray[entityCount]["transformComponent"]["scale"]["x"] =
        tc.getScale().x;
      serializedArray[entityCount]["transformComponent"]["scale"]["y"] =
        tc.getScale().y;
      serializedArray[entityCount]["transformComponent"]["scale"]["z"] =
        tc.getScale().z;

      if (entity.HasComponent<LightComponent>())
      {
        LightComponent lightComponent = entity.GetComponent<LightComponent>();
        serializedArray[entityCount]["lightComponent"]["type"] =
          (int)lightComponent.type;

        serializedArray[entityCount]["lightComponent"]["lightColor"]["r"] =
          (int)lightComponent.lightColor.r;
        serializedArray[entityCount]["lightComponent"]["lightColor"]["g"] =
          (int)lightComponent.lightColor.g;
        serializedArray[entityCount]["lightComponent"]["lightColor"]["b"] =
          (int)lightComponent.lightColor.b;

        serializedArray[entityCount]["lightComponent"]["attenuation"] =
          (int)lightComponent.attenuation;

        serializedArray[entityCount]["lightComponent"]["radius"] =
          (int)lightComponent.radius;

        serializedArray[entityCount]["lightComponent"]["openingAngle"] =
          (int)lightComponent.openingAngle;
      }

      if (entity.HasComponent<MeshRendererComponent>())
      {
        MeshRendererComponent meshRendererComponent =
          entity.GetComponent<MeshRendererComponent>();
        if (meshRendererComponent.meshAsset)
        {
          serializedArray[entityCount]["meshRendererComponent"]["mesh"] =
            (uint64_t)(*meshRendererComponent.meshAsset);
        }
        else
        {
          serializedArray[entityCount]["meshRendererComponent"]["mesh"] =
            "null";
        }

        int materialCount = 0;

        for (std::shared_ptr<UID> materialID :
             meshRendererComponent.materialAssets)
        {
          serializedArray[entityCount]["meshRendererComponent"]["materials"]
                         [materialCount] =
                           materialID ? (uint64_t)(*materialID) : -1;
          materialCount++;
        }

        serializedArray[entityCount]["meshRendererComponent"]
                       ["canCastShadows"] = meshRendererComponent.canCastShadow;
      }

      serializedArray[entityCount]["children"] =
        SerializeEntities(tc.children, scene);

      entityCount++;
    }*/

    return serializedScene;
  }
}