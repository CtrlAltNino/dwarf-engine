#pragma once

#include <entt/entt.hpp>

#include "Core/Base.h"
#include "Core/UID.h"
#include "Core/GenericComponents.h"
//#include "Core/Asset/AssetComponents.h"
//#include "Core/Scene/Components.h"

namespace Dwarf {

    /// @brief Enum representing the types of assets.
    //enum class AssetType {Unknown, Mesh, Material, Texture, VertexShader, FragmentShader, TesselationControlShader, TesselationEvaluationShader, GeometryShader, ComputeShader};

    /// @brief Wrapper of an entity. It handles the access of the components.
    template <typename T>
    class AssetReference {
        private:
            /// @brief Registry handle of the corresponding entity.
            entt::entity m_AssetHandle;

            /// @brief Pointer to the holder of the ECS registry.
            Ref<entt::registry> m_Registry;
        public:
            AssetReference(entt::entity assetHandle, Ref<entt::registry> registry): m_AssetHandle(assetHandle), m_Registry(registry){ }

            AssetReference(entt::entity assetHandle, std::string assetName, Ref<entt::registry> registry, UID uid, std::filesystem::path assetPath): m_AssetHandle(assetHandle), m_Registry(registry){
                m_Registry->emplace<T>(assetHandle, assetPath);
                m_Registry->emplace<IDComponent>(assetHandle, uid);
                m_Registry->emplace<TagComponent>(assetHandle, assetName);
                m_Registry->emplace<PathComponent>(assetHandle, assetPath);
            }
            //AssetReference(const AssetReference& other) = default;

            operator bool() const { return (std::uint32_t)m_AssetHandle != 0; }
            bool operator ==(const AssetReference &b){return m_AssetHandle == b.m_AssetHandle;}

            /*template<typename T, typename... Args>
            T& AddComponent(Args&&... args){
                // TODO: Check component requirements
                return m_Registry->emplace<T>(m_AssetHandle, std::forward<Args>(args)...);
            }*/

            /// @brief Returns the component of a given type.
            /// @tparam T Type of component.
            /// @return The found component.
            /*template<typename T>
            T& GetComponent(){
                // TODO: Check if component present
                return m_Registry->get<T>(m_AssetHandle);
            }*/

            /// @brief Retrieves the handle of the asset entity.
            /// @return The handle.
            entt::entity GetHandle() { return m_AssetHandle; }

            /// @brief Returns the UID of the asset.
            /// @return The UID.
            Ref<UID> GetUID(){
                return m_Registry->get<IDComponent>(m_AssetHandle).ID;
            }

            std::filesystem::path GetPath(){
                return m_Registry->get<PathComponent>(m_AssetHandle).Path;
            }
            
            /// @brief Retrieves the type of the asset.
            /// @return Asset type.
            /*AssetType GetType() {
                return m_Type;
            }*/

            /// @brief Retrieves the asset component of the asset, containing the actual payload.
            /// @return The asset component.
            T GetAsset() {
                return m_Registry->get<T>(m_AssetHandle);
                /*switch(m_Type){
                    case AssetType::Mesh:
                        return (void*)&m_Registry->get<MeshAsset>(m_AssetHandle);
                        break;
                    case AssetType::Material:
                        return (void*)&m_Registry->get<MaterialAsset>(m_AssetHandle);
                        break;
                    case AssetType::Texture:
                        return (void*)&m_Registry->get<TextureAsset>(m_AssetHandle);
                        break;
                    case AssetType::VertexShader:
                        return (void*)&m_Registry->get<VertexShaderAsset>(m_AssetHandle);
                        break;
                    case AssetType::TesselationControlShader:
                        return (void*)&m_Registry->get<TesselationControlShaderAsset>(m_AssetHandle);
                        break;
                    case AssetType::TesselationEvaluationShader:
                        return (void*)&m_Registry->get<TesselationEvaluationShaderAsset>(m_AssetHandle);
                        break;
                    case AssetType::GeometryShader:
                        return (void*)&m_Registry->get<GeometryShaderAsset>(m_AssetHandle);
                        break;
                    case AssetType::FragmentShader:
                        return (void*)&m_Registry->get<FragmentShaderAsset>(m_AssetHandle);
                        break;
                    case AssetType::ComputeShader:
                        return (void*)&m_Registry->get<ComputeShaderAsset>(m_AssetHandle);
                        break;
                    case AssetType::Unknown:
                        return nullptr;
                        break;
                }*/
            }
    };
}