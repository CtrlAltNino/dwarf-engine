#pragma once
#include "../../utilities/dpch.h"

#include"../components/Components.h"
#include"../scene/Object.h"
#include"../scene/Camera.h"

#include<glad/glad.h>
#include<entt/entt.hpp>
#include<entt/entity/registry.hpp>

#include"../editor/UID.h"
#include"../components/Components.h"
#include"../scene/EntityProvider.h"

class Entity {
    public:
        bool active;
        
        Entity(entt::entity handle, EntityProvider* entProvider);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args){
            // TODO: Check component requirements
            return entProvider->m_Registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent(){
            // TODO: Check if component present
            return entProvider->m_Registry.get<T>(entityHandle);
        }

        template<typename T>
        void RemoveComponent(){
            // TODO: Check if component is present
            //if(HasComponent<T>()){
                entProvider->m_Registry.remove<T>(entityHandle);
            //}
        }

        template<typename T>
        bool HasComponent(){
            //entt::registry reg = entProvider->m_Registry;
            //return reg.has<T>(entityHandle);
            return false;
        }

        UID GetUID() { return GetComponent<IDComponent>().ID; }

        operator bool() const { return (std::uint32_t)entityHandle != 0; }

        void SetParent(entt::entity entity){
            TransformComponent transform = GetComponent<TransformComponent>();
            Entity newParent = Entity(entity, entProvider);
            
            if(entProvider->m_Registry.valid(transform.parent)){
                Entity oldParent = Entity(transform.parent, entProvider);
                oldParent.RemoveChild(entityHandle);
            }
            transform.parent = entity;
            RemoveComponent<TransformComponent>();
            AddComponent<TransformComponent>(transform);
            newParent.AddChild(entityHandle);
        }
        
        void AddChild(entt::entity entity){
            TransformComponent transform = GetComponent<TransformComponent>();

            if(transform.children.empty() || (std::find(transform.children.begin(), transform.children.end(), entity) == transform.children.end())){
                GetComponent<TransformComponent>().children.push_back(entity);
            }
        }
        
        void RemoveChild(entt::entity entity){
            TransformComponent transform = GetComponent<TransformComponent>();
            auto it = std::find(transform.children.begin(), transform.children.end(), entity);
            if(it != transform.children.end()){
                transform.children.erase(it);
            }
        }

        /*entt::entity GetParent(){
            return
        }
        std::vector<entt::entity> GetChildren();*/
        
        entt::entity GetHandle() { return entityHandle; }
    private:
        EntityProvider* entProvider = nullptr;
        entt::entity entityHandle{ entt::null };
        
        Entity* parent = nullptr;
        std::vector<Entity*> children;
        
        //entt::registry registry;
};