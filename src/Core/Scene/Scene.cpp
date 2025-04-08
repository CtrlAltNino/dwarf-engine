#include "pch.h"

#include "Core/Scene/Components/SceneComponents.h"
#include "Core/Scene/Scene.h"

namespace Dwarf
{
  Scene::Scene(std::unique_ptr<ISceneProperties> properties,
               std::shared_ptr<IAssetDatabase>   assetDatabase)
    : mRegistry(entt::registry())
    , mProperties(std::move(properties))
    , mRootEntity(CreateRootEntity())
    , mAssetDatabase(std::move(assetDatabase))
  {
  }

  Scene::Scene(const SerializedGraph&            serializedScene,
               std::unique_ptr<ISceneProperties> properties,
               std::shared_ptr<IAssetDatabase>   assetDatabase)
    : mRegistry(entt::registry())
    , mProperties(std::move(properties))
    , mRootEntity(CreateRootEntity())
    , mAssetDatabase(std::move(assetDatabase))
  {
    Deserialize(serializedScene.t);
  }

  Scene::~Scene()
  {
    // TODO: Clear registry
    // mRegistry.clear();
  }

  void
  Scene::Deserialize(const nlohmann::json& serializedSceneGraph)
  {
    for (auto const& element : serializedSceneGraph)
    {
      Entity newEntity = DeserializeEntity(element);
      newEntity.SetParent(mRootEntity.GetHandle());
    }
  }

  auto
  Scene::DeserializeEntity(const nlohmann::json& serializedEntity) -> Entity
  {
    Entity newEntity =
      CreateEntityWithUID(UUID(serializedEntity["guid"].get<std::string>()),
                          serializedEntity["name"]);

    auto& transformComponent = newEntity.GetComponent<TransformComponent>();
    transformComponent.GetPosition() = {
      serializedEntity["TransformComponent"]["Position"]["x"],
      serializedEntity["TransformComponent"]["Position"]["y"],
      serializedEntity["TransformComponent"]["Position"]["z"]
    };
    transformComponent.GetEulerAngles() = {
      serializedEntity["TransformComponent"]["Rotation"]["x"],
      serializedEntity["TransformComponent"]["Rotation"]["y"],
      serializedEntity["TransformComponent"]["Rotation"]["z"]
    };
    transformComponent.GetScale() = {
      serializedEntity["TransformComponent"]["Scale"]["x"],
      serializedEntity["TransformComponent"]["Scale"]["y"],
      serializedEntity["TransformComponent"]["Scale"]["z"]
    };

    if (serializedEntity.contains("LightComponent"))
    {
      newEntity.AddComponent<LightComponent>(
        serializedEntity["LightComponent"]);
    }

    if (serializedEntity.contains("MeshRendererComponent"))
    {
      std::unique_ptr<IAssetReference> modelAsset = nullptr;

      if (!serializedEntity["MeshRendererComponent"]["Model"]
             .get<std::string>()
             .empty())
      {
        modelAsset = mAssetDatabase->Retrieve(
          UUID(serializedEntity["MeshRendererComponent"]["Model"]
                 .get<std::string>()));
      }

      std::map<int, std::unique_ptr<IAssetReference>> materialAssets;

      if (serializedEntity["MeshRendererComponent"].contains("Materials"))
      {
        for (auto it =
               serializedEntity["MeshRendererComponent"]["Materials"].begin();
             it != serializedEntity["MeshRendererComponent"]["Materials"].end();
             ++it)
        {
          int index = std::stoi(it.key());
          if (!it.value().get<std::string>().empty() &&
              (it.value().get<std::string>() != "null"))
          {
            materialAssets[index] =
              mAssetDatabase->Retrieve(UUID(it.value().get<std::string>()));
          }
          else
          {
            materialAssets[index] = nullptr;
          }
        }
      }

      auto& comp = newEntity.AddComponent<MeshRendererComponent>(
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

  auto
  Scene::CreateRootEntity() -> Entity
  {
    Entity mRootEntity(mRegistry.create(), mRegistry);
    mRootEntity.AddComponent<IDComponent>(UUID());
    mRootEntity.AddComponent<TransformComponent>();
    mRootEntity.AddComponent<NameComponent>("Root");
    return mRootEntity;
  }

  auto
  Scene::CreateEntity(const std::string& name) -> Entity
  {
    return CreateEntityWithUID(UUID(), name);
  }

  auto
  Scene::CreateEntityWithUID(const UUID& uid, const std::string& name) -> Entity
  {
    Entity entity(mRegistry.create(), mRegistry);
    entity.AddComponent<IDComponent>(uid);
    entity.AddComponent<TransformComponent>();
    auto& nameComp = entity.AddComponent<NameComponent>(name);
    nameComp.Name = name.empty() ? "Entity" : name;

    entity.SetParent(mRootEntity.GetHandle());

    return entity;
  }

  auto
  Scene::GetRegistry() -> entt::registry&
  {
    return mRegistry;
  }

  auto
  Scene::GetRootEntity() -> Entity&
  {
    return mRootEntity;
  }

  auto
  Scene::GetProperties() -> ISceneProperties&
  {
    return *mProperties;
  }

  auto
  Scene::GetFullModelMatrix(TransformComponent& transform) -> glm::mat4
  {
    auto                   parentMat = glm::mat4(1.0F);
    std::vector<glm::mat4> matriceStack;

    matriceStack.push_back(transform.GetModelMatrix());

    if (entt::entity cursor = transform.GetParent(); cursor != entt::null)
    {
      Entity cur(cursor, mRegistry);
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
    if (mRegistry.valid(entity.GetHandle()))
    {
      if (!entity.GetChildren().empty())
      {
        for (entt::entity child : entity.GetChildren())
        {
          DeleteEntity(Entity(child, mRegistry));
        }
      }

      Entity(entity.GetParent(), mRegistry).RemoveChild(entity.GetHandle());
      mRegistry.destroy(entity.GetHandle());
    }
  }

  auto
  Scene::Serialize() -> nlohmann::json
  {
    nlohmann::json serializedScene;

    serializedScene["Settings"] = mProperties->Serialize();

    // TODO: Serialize Scene
    serializedScene["Graph"] = nlohmann::json::array();

    for (entt::entity entity : mRegistry.view<IDComponent>())
    {
      Entity e(entity, mRegistry);
      if (e.GetParent() == mRootEntity.GetHandle())
      {
        serializedScene["Graph"].push_back(e.Serialize());
      }
    }

    return serializedScene;
  }
}