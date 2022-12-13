#pragma once
#include "../../utilities/dpch.h"

#include"../components/Components.h"
//#include"../scene/Object.h"
#include"../scene/Camera.h"

#include<glad/glad.h>
#include<entt/entt.hpp>
#include<entt/entity/registry.hpp>
#include<algorithm>

#include"../editor/UID.h"
#include"../components/Components.h"
#include"../scene/EntityProvider.h"

/// @brief Class wrapping around an entity, providing gameobject functionality.
class Entity {
    public:
        Entity(entt::entity handle, EntityProvider* entProvider);
        Entity(const Entity& other) = default;

        /// @brief Adds a component to the entity.
        /// @tparam T Type of component.
        /// @tparam ...Args Component arguments.
        /// @param ...args Components to add.
        /// @return The added component.
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args){
            // TODO: Check component requirements
            return entProvider->m_Registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
        }

        /// @brief Retrieves a component of the Entity if present.
        /// @tparam T Type of component.
        /// @return The found component.
        template<typename T>
        T& GetComponent(){
            // TODO: Check if component present
            return entProvider->m_Registry.get<T>(entityHandle);
        }

        /// @brief Removes a component from the entity.
        /// @tparam T Type of component to remove.
        template<typename T>
        void RemoveComponent(){
            // TODO: Check if component is present
            if(HasComponent<T>()){
                entProvider->m_Registry.remove<T>(entityHandle);
            }
        }

        /// @brief Checks if the entity has a component assigned.
        /// @tparam T Type of component to check for.
        /// @return True if component is present, false if not.
        template<typename T>
        bool HasComponent(){
            return entProvider->m_Registry.try_get<T>(entityHandle) != nullptr;
        }

        /// @brief Retrieves the UID of the entity.
        /// @return The UID.
        UID GetUID() { return GetComponent<IDComponent>().ID; }

        operator bool() const { return (std::uint32_t)entityHandle != 0; }
        bool operator ==(const Entity &b){return entityHandle == b.entityHandle;}

        /// @brief Sets a new parent for this entity.
        /// @param entity The entity handle of the parent-to-be.
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
        
        /// @brief Adds a new child entity to this entity.
        /// @param entity Entity handle of the new child.
        void AddChild(entt::entity entity){
            TransformComponent transform = GetComponent<TransformComponent>();

            if(transform.children.empty() || (std::find(transform.children.begin(), transform.children.end(), entity) == transform.children.end())){
                GetComponent<TransformComponent>().children.push_back(entity);
            }
        }
        
        /// @brief Removes a child from the list of children.
        /// @param entity Entity handle of the entity to remove.
        void RemoveChild(entt::entity entity){
            //TransformComponent transform = GetComponent<TransformComponent>();
            auto it = std::find(GetComponent<TransformComponent>().children.begin(), GetComponent<TransformComponent>().children.end(), entity);
            if(it != GetComponent<TransformComponent>().children.end()){
                GetComponent<TransformComponent>().children.erase(it);
            }
        }

        /// @brief Sets the position this entity should take between its siblings.
        /// @param index Index for the list of childrens of this entity's parent. (Clamped to the number of children)
        void SetChildIndex(int index){
            std::vector<entt::entity>* siblings = &Entity(GetComponent<TransformComponent>().parent, entProvider).GetComponent<TransformComponent>().children;
            auto it = std::find(siblings->begin(), siblings->end(), entityHandle);

            //std::iter_swap(siblings->begin() + index, it);
            siblings->erase(it);

            if(index >= siblings->size()){
                siblings->push_back(entityHandle);
            }else{
                siblings->insert(siblings->begin()+index, entityHandle);
            }
        }

        /// @brief Returns the index of this entity in the list of children of this entity's parent.
        /// @return Index of this entity.
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

        /// @brief Returns the entity handle of this entity's parent.
        /// @return Entity handle of the parent.
        entt::entity GetParent(){
            return GetComponent<TransformComponent>().parent;
        }
        
        /// @brief Returns the list of this entity's children.
        /// @return Pointer to the list of child entities.
        std::vector<entt::entity>* GetChildren(){
            return &(GetComponent<TransformComponent>().children);
        }
        
        entt::entity GetHandle() { return entityHandle; }
    private:
        /// @brief Pointer to a holder of an ECS registry.
        EntityProvider* entProvider = nullptr;

        /// @brief The entity handle of this entity.
        entt::entity entityHandle{ entt::null };
        
        //Entity* parent = nullptr;
        //std::vector<Entity*> children;
        
        //entt::registry registry;
};