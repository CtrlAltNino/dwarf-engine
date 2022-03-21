#pragma once

//#include"../editor/UUID.h"
//#include"../components/Components.h"
#include"../scene/Scene.h"
#include<entt/entt.hpp>

class Entity {
    public:
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args){
            // TODO: Check component requirements
            return scene->m_Registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent(){
            // TODO: Check if component present
            return scene->m_Registry.get<T>(entityHandle);
        }

        template<typename T>
        void RemoveComponent(){
            // TODO: Check if component is present
            scene->m_Registry.remove<T>(entityHandle);
        }

        template<typename T>
        bool HasComponent(){
            return scene->m_registry->has<T>(entityHandle);
        }

        //UUID GetUUID() { return GetComponent<IDComponent>().ID; }

        operator bool() const { return (std::uint32_t)entityHandle != 0; }
    private:
        entt::entity entityHandle{ 0 };
        Scene* scene = nullptr;
};