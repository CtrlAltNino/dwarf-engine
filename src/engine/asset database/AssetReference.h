#pragma once

#include"../../utilities/dpch.h"
#include"../editor/UID.h"
#include"../scene/EntityProvider.h"
#include"AssetComponents.h"
#include"../components/Components.h"

#include<entt/entt.hpp>

enum class AssetType {Unknown, Mesh, Material, Texture, VertexShader, FragmentShader, TesselationControlShader, TesselationEvaluationShader, GeometryShader, ComputeShader};

class AssetReference : public EntityProvider{
    private:
        entt::entity assetHandle;
        AssetType type;
        EntityProvider* entProvider = nullptr;
    public:
        AssetReference(entt::entity assetHandle, EntityProvider* entProvider);
        //AssetReference(const AssetReference& other) = default;

        operator bool() const { return (std::uint32_t)assetHandle != 0; }
        bool operator ==(const AssetReference &b){return assetHandle == b.assetHandle;}

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args){
            // TODO: Check component requirements
            return entProvider->m_Registry.emplace<T>(assetHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        T& GetComponent(){
            // TODO: Check if component present
            return entProvider->m_Registry.get<T>(assetHandle);
        }

        entt::entity GetHandle() { return assetHandle; }

        UID GetUID() { return GetComponent<IDComponent>().ID; }
        
        AssetType GetType() {
            return type;
        }

        void* GetAsset() {
            switch(type){
                case AssetType::Mesh:
                    return (void*)&entProvider->m_Registry.get<MeshAsset>(assetHandle);
                    break;
                case AssetType::Material:
                    return (void*)&entProvider->m_Registry.get<MaterialAsset>(assetHandle);
                    break;
                case AssetType::Texture:
                    return (void*)&entProvider->m_Registry.get<TextureAsset>(assetHandle);
                    break;
                case AssetType::VertexShader:
                    return (void*)&entProvider->m_Registry.get<VertexShaderAsset>(assetHandle);
                    break;
                case AssetType::TesselationControlShader:
                    return (void*)&entProvider->m_Registry.get<TesselationControlShaderAsset>(assetHandle);
                    break;
                case AssetType::TesselationEvaluationShader:
                    return (void*)&entProvider->m_Registry.get<TesselationEvaluationShaderAsset>(assetHandle);
                    break;
                case AssetType::GeometryShader:
                    return (void*)&entProvider->m_Registry.get<GeometryShaderAsset>(assetHandle);
                    break;
                case AssetType::FragmentShader:
                    return (void*)&entProvider->m_Registry.get<FragmentShaderAsset>(assetHandle);
                    break;
                case AssetType::ComputeShader:
                    return (void*)&entProvider->m_Registry.get<ComputeShaderAsset>(assetHandle);
                    break;
                case AssetType::Unknown:
                    return nullptr;
                    break;
            }
        }
};