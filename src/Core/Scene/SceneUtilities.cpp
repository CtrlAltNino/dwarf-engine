#include "dpch.h"

#include "Core/Scene/SceneUtilities.h"

#include <nfd.h>

#include "Editor/Editor.h"
#include "Core/Asset/AssetDatabase.h"

namespace Dwarf {
    void SceneUtilities::SetLastOpenedScene(std::filesystem::path path){
        std::filesystem::path projectSettingsPath = Editor::Get().GetModel()->GetProjectPath() / "projectSettings.dproj";
        nlohmann::json projectSettings = nlohmann::json::parse(FileHandler::ReadFile(projectSettingsPath));

        projectSettings["projectInformation"]["lastOpenedScene"] = (uint64_t)(*AssetDatabase::Retrieve<SceneAsset>(path)->GetUID());

        FileHandler::WriteToFile(projectSettingsPath, projectSettings.dump(4));
    }

    Ref<Scene> SceneUtilities::Deserialize(std::filesystem::path path){
        nlohmann::json serializedScene = nlohmann::json::parse(FileHandler::ReadFile(path));

        SceneSettings settings;
        
        settings.fogSettings.fogColor = {
                serializedScene["fog"]["color"]["r"],
                serializedScene["fog"]["color"]["g"],
                serializedScene["fog"]["color"]["b"]
        };
        settings.fogSettings.fogStart = serializedScene["fog"]["fogStart"];
        settings.fogSettings.fogEnd = serializedScene["fog"]["fogEnd"];
        settings.fogSettings.type = (FogType)serializedScene["fog"]["type"];

        settings.globalLightSettings.color = {
            serializedScene["globalLight"]["color"]["r"],
            serializedScene["globalLight"]["color"]["g"],
            serializedScene["globalLight"]["color"]["b"]
        };
        settings.globalLightSettings.intensity = serializedScene["globalLight"]["intensity"];

        if(serializedScene.contains("skyboxMaterial") && serializedScene["skyboxMaterial"] != "null"){
            settings.skyboxMaterial = CreateRef<UID>(UID((uint64_t)serializedScene["skyboxMaterial"]));
        }

        Ref<Scene> deserializedScene = CreateRef<Scene>(Scene(path, settings));

        for (auto& element : serializedScene["hierarchy"]) {
            Entity newEntity = DeserializeEntity(element, deserializedScene);
            newEntity.SetParent(deserializedScene->GetRootEntity()->GetHandle());
        }

        return deserializedScene;
    }

    void SceneUtilities::Serialize(Ref<Scene> scene){
        nlohmann::json serializedScene;

        // Serializing Hierarchy
        serializedScene["hierarchy"] = SerializeEntities(scene->GetRootEntity()->GetComponent<TransformComponent>().children, scene);

        SceneSettings settings = scene->GetSettings();

        // Serializing Fog settings
        serializedScene["fog"]["color"]["r"] = settings.fogSettings.fogColor.r;
        serializedScene["fog"]["color"]["g"] = settings.fogSettings.fogColor.g;
        serializedScene["fog"]["color"]["b"] = settings.fogSettings.fogColor.b;

        serializedScene["fog"]["fogStart"] = settings.fogSettings.fogStart;
        serializedScene["fog"]["fogEnd"] = settings.fogSettings.fogEnd;

        serializedScene["fog"]["type"] = (int)settings.fogSettings.type;

        // Serializing Global lighting settings
        serializedScene["globalLight"]["color"]["r"] = settings.globalLightSettings.color.r;
        serializedScene["globalLight"]["color"]["g"] = settings.globalLightSettings.color.g;
        serializedScene["globalLight"]["color"]["b"] = settings.globalLightSettings.color.b;

        serializedScene["globalLight"]["intensity"] = settings.globalLightSettings.intensity;

        // Serializing Skybox settings
        if(settings.skyboxMaterial){
            serializedScene["skyboxMaterial"] = (uint64_t)(*settings.skyboxMaterial);
        }else{
            serializedScene["skyboxMaterial"] = "null";
        }

        FileHandler::WriteToFile(scene->GetPath(), serializedScene.dump(4));
    }

    nlohmann::json SceneUtilities::SerializeEntities(std::vector<entt::entity> entities, Ref<Scene> scene){
        int entityCount = 0;
        nlohmann::json serializedArray;
        for(auto entityHandle: entities) {
            Entity entity(entityHandle, scene->GetRegistry());

            serializedArray[entityCount]["guid"] = (uint64_t)(*entity.GetComponent<IDComponent>().ID);
            serializedArray[entityCount]["name"] = entity.GetComponent<TagComponent>().Tag;

            TransformComponent tc = entity.GetComponent<TransformComponent>();
            serializedArray[entityCount]["transformComponent"]["position"]["x"] = tc.getPosition().x;
            serializedArray[entityCount]["transformComponent"]["position"]["y"] = tc.getPosition().y;
            serializedArray[entityCount]["transformComponent"]["position"]["z"] = tc.getPosition().z;

            serializedArray[entityCount]["transformComponent"]["rotation"]["x"] = tc.getEulerAngles().x;
            serializedArray[entityCount]["transformComponent"]["rotation"]["y"] = tc.getEulerAngles().y;
            serializedArray[entityCount]["transformComponent"]["rotation"]["z"] = tc.getEulerAngles().z;

            serializedArray[entityCount]["transformComponent"]["scale"]["x"] = tc.getScale().x;
            serializedArray[entityCount]["transformComponent"]["scale"]["y"] = tc.getScale().y;
            serializedArray[entityCount]["transformComponent"]["scale"]["z"] = tc.getScale().z;

            if(entity.HasComponent<LightComponent>()){
                LightComponent lightComponent = entity.GetComponent<LightComponent>();
                serializedArray[entityCount]["lightComponent"]["type"] = (int)lightComponent.type;

                serializedArray[entityCount]["lightComponent"]["lightColor"]["r"] = (int)lightComponent.lightColor.r;
                serializedArray[entityCount]["lightComponent"]["lightColor"]["g"] = (int)lightComponent.lightColor.g;
                serializedArray[entityCount]["lightComponent"]["lightColor"]["b"] = (int)lightComponent.lightColor.b;

                serializedArray[entityCount]["lightComponent"]["attenuation"] = (int)lightComponent.attenuation;

                serializedArray[entityCount]["lightComponent"]["radius"] = (int)lightComponent.radius;

                serializedArray[entityCount]["lightComponent"]["openingAngle"] = (int)lightComponent.openingAngle;
            }

            if(entity.HasComponent<MeshRendererComponent>()){
                MeshRendererComponent meshRendererComponent = entity.GetComponent<MeshRendererComponent>();
                if(meshRendererComponent.mesh){
                    serializedArray[entityCount]["meshRendererComponent"]["mesh"] = (uint64_t)(*meshRendererComponent.mesh);
                }else{
                    serializedArray[entityCount]["meshRendererComponent"]["mesh"] = "null";
                }
                
                int materialCount = 0;
                
                for(Ref<UID> materialID : meshRendererComponent.materials){
                    serializedArray[entityCount]["meshRendererComponent"]["materials"][materialCount++] = (uint64_t)(*materialID);
                }

                serializedArray[entityCount]["meshRendererComponent"]["canCastShadows"] = meshRendererComponent.canCastShadow;
            }

            serializedArray[entityCount]["children"] = SerializeEntities(tc.children, scene);

            entityCount++;
        }

        return serializedArray;
    }

    Entity SceneUtilities::DeserializeEntity(nlohmann::json serializedEntity, Ref<Scene> scene){
        Entity newEntity = scene->CreateEntityWithUID(UID((uint64_t)serializedEntity["guid"]), serializedEntity["name"]);

        TransformComponent &tc = newEntity.GetComponent<TransformComponent>();
        tc.position = {
            serializedEntity["transformComponent"]["position"]["x"],
            serializedEntity["transformComponent"]["position"]["y"],
            serializedEntity["transformComponent"]["position"]["z"]
        };
        tc.rotation = {
            serializedEntity["transformComponent"]["rotation"]["x"],
            serializedEntity["transformComponent"]["rotation"]["y"],
            serializedEntity["transformComponent"]["rotation"]["z"]
        };
        tc.scale = {
            serializedEntity["transformComponent"]["scale"]["x"],
            serializedEntity["transformComponent"]["scale"]["y"],
            serializedEntity["transformComponent"]["scale"]["z"]
        };

        if(serializedEntity.contains("lightComponent")){
            LightComponent &lightComponent = newEntity.AddComponent<LightComponent>();
            lightComponent.type = (LightComponent::LIGHT_TYPE)serializedEntity["lightComponent"]["type"];
            lightComponent.lightColor = {
                serializedEntity["lightComponent"]["lightColor"]["r"],
                serializedEntity["lightComponent"]["lightColor"]["g"],
                serializedEntity["lightComponent"]["lightColor"]["b"]
            };

            lightComponent.attenuation = serializedEntity["lightComponent"]["attenuation"];

            lightComponent.radius = serializedEntity["lightComponent"]["radius"];

            lightComponent.openingAngle = serializedEntity["lightComponent"]["openingAngle"];
        }

        if(serializedEntity.contains("meshRendererComponent")){
            MeshRendererComponent &meshRendererComponent = newEntity.AddComponent<MeshRendererComponent>();

            meshRendererComponent.canCastShadow = (bool)serializedEntity["meshRendererComponent"]["canCastShadows"];

            if(serializedEntity["meshRendererComponent"].contains("mesh") && serializedEntity["meshRendererComponent"]["mesh"] != "null"){
                meshRendererComponent.mesh = CreateRef<UID>(UID((uint64_t)serializedEntity["meshRendererComponent"]["mesh"]));
            }

            if(serializedEntity["meshRendererComponent"].contains("materials")){
                for (auto& element : serializedEntity["meshRendererComponent"]["materials"]){
                    meshRendererComponent.materials.push_back(CreateRef<UID>(UID((uint64_t)element)));
                }
            }
        }

        for (auto& element : serializedEntity["children"]) {
            Entity childEntity = DeserializeEntity(element, scene);
            childEntity.SetParent(newEntity.GetHandle());
        }

        return newEntity;
    }

    void SceneUtilities::SaveScene(Ref<Scene> scene){
        if(!scene->GetPath().empty()){
            Serialize(scene);
        }else{
            SaveSceneDialog(scene);
        }
    }

    void SceneUtilities::SaveSceneDialog(Ref<Scene> scene){
        nfdchar_t *savePath = NULL;
        nfdresult_t result = NFD_SaveDialog( "dscene", AssetDatabase::GetAssetDirectoryPath().string().c_str(), &savePath );
        if ( result == NFD_OKAY )
        {
            //puts("Success!");
            //puts(savePath);
            std::filesystem::path path(savePath);

            if(!path.has_extension()){
                path.concat(".dscene");
            }
            
            scene->SetPath(path);
            Serialize(scene);
            free(savePath);
        }
        else if ( result == NFD_CANCEL )
        {
            //puts("User pressed cancel.");
        }
        else 
        {
            printf("Error: %s\n", NFD_GetError() );
        }
    }

    Ref<Scene> SceneUtilities::LoadScene(std::filesystem::path path){
        Ref<Scene> scene = Deserialize(path);

        return scene;
    }

    Ref<Scene> SceneUtilities::LoadSceneDialog(){
        Ref<Scene> scene;
        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog( "dscene", AssetDatabase::GetAssetDirectoryPath().string().c_str(), &outPath );
        if ( result == NFD_OKAY )
        {
            //puts("Success!");
            //puts(outPath);
            std::filesystem::path path(outPath);
            scene = LoadScene(path);
            free(outPath);
        }
        else if ( result == NFD_CANCEL )
        {
            //puts("User pressed cancel.");
        }
        else 
        {
            printf("Error: %s\n", NFD_GetError() );
        }

        return scene;
    }

    Ref<Scene> SceneUtilities::LoadDefaultScene(){
        Ref<Scene> scene = CreateRef<Scene>(Scene(std::filesystem::path(""), SceneSettings()));

        // TODO Add default stuff

        return scene;
    }
}