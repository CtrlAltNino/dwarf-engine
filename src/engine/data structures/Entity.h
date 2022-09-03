#pragma once
#include "../../utilities/dpch.h"

#include"../components/Components.h"
#include"../scene/Object.h"
#include"../scene/Camera.h"

#include<glad/glad.h>
#include<entt/entt.hpp>
#include<entt/entity/registry.hpp>
#include<algorithm>

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
        bool operator ==(const Entity &b){return entityHandle == b.entityHandle;}

        void SetParent(entt::entity entity){
            TransformComponent transform = GetComponent<TransformComponent>();
            Entity newParent = Entity(entity, entProvider);
            
            if(transform.parent != entt::null){
                Entity oldParent = Entity(transform.parent, entProvider);
                //std::cout << "Old Parent: " << oldParent.GetComponent<TagComponent>().Tag << std::endl;
                oldParent.RemoveChild(entityHandle);
            }

            GetComponent<TransformComponent>().parent = entity;
            if(entProvider->m_Registry.valid(entity)){
                newParent.AddChild(entityHandle);
            }
        }
        
        void AddChild(entt::entity entity){
            TransformComponent transform = GetComponent<TransformComponent>();

            if(transform.children.empty() || (std::find(transform.children.begin(), transform.children.end(), entity) == transform.children.end())){
                GetComponent<TransformComponent>().children.push_back(entity);
            }
        }
        
        void RemoveChild(entt::entity entity){
            //TransformComponent transform = GetComponent<TransformComponent>();
            auto it = std::find(GetComponent<TransformComponent>().children.begin(), GetComponent<TransformComponent>().children.end(), entity);
            if(it != GetComponent<TransformComponent>().children.end()){
                GetComponent<TransformComponent>().children.erase(it);
            }
        }

        void SetChildIndex(int index){
            std::vector<entt::entity>* siblings = &Entity(GetComponent<TransformComponent>().parent, entProvider).GetComponent<TransformComponent>().children;
            auto it = std::find(siblings->begin(), siblings->end(), entityHandle);

            //std::iter_swap(siblings->begin() + index, it);
            siblings->erase(it);
            siblings->insert(siblings->begin()+index, entityHandle);
        }

        int GetChildIndex(){
            int index = -1;
            
            std::vector<entt::entity> siblings = Entity(GetComponent<TransformComponent>().parent, entProvider).GetComponent<TransformComponent>().children;
            auto it = std::find(siblings.begin(), siblings.end(), entityHandle);

            // If element was found
            if (it != siblings.end()) 
            {
                index = it - siblings.begin();
            }

            return index;
        }

        entt::entity GetParent(){
            return GetComponent<TransformComponent>().parent;
        }
        
        std::vector<entt::entity>* GetChildren(){
            return &(GetComponent<TransformComponent>().children);
        }
        
        entt::entity GetHandle() { return entityHandle; }
    private:
        EntityProvider* entProvider = nullptr;
        entt::entity entityHandle{ entt::null };
        
        //Entity* parent = nullptr;
        //std::vector<Entity*> children;
        
        //entt::registry registry;
};