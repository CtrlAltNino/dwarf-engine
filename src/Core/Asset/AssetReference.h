#pragma once

#include <entt/entt.hpp>

#include "Core/UID.h"
#include "Core/Scene/EntityProvider.h"
#include "Core/Asset/AssetComponents.h"
#include "Core/Scene/Components.h"

namespace Dwarf {
    
    /// @brief Enum representing the types of assets.
    enum class AssetType {Unknown, Mesh, Material, Texture, VertexShader, FragmentShader, TesselationControlShader, TesselationEvaluationShader, GeometryShader, ComputeShader};

    /// @brief Wrapper of an entity. It handles the access of the components.
    class AssetReference : public EntityProvider{
        private:
            /// @brief Registry handle of the corresponding entity.
            entt::entity assetHandle;

            /// @brief Type of the asset.
            AssetType type;

            /// @brief Pointer to the holder of the ECS registry.
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

            /// @brief Returns the component of a given type.
            /// @tparam T Type of component.
            /// @return The found component.
            template<typename T>
            T& GetComponent(){
                // TODO: Check if component present
                return entProvider->m_Registry.get<T>(assetHandle);
            }

            /// @brief Retrieves the handle of the asset entity.
            /// @return The handle.
            entt::entity GetHandle() { return assetHandle; }

            /// @brief Returns the UID of the asset.
            /// @return The UID.
            UID GetUID() { return GetComponent<IDComponent>().ID; }
            
            /// @brief Retrieves the type of the asset.
            /// @return Asset type.
            AssetType GetType() {
                return type;
            }

            /// @brief Retrieves the asset component of the asset, containing the actual payload.
            /// @return The asset component.
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
}