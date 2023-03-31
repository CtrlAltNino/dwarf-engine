#pragma once

#include <nlohmann/json.hpp>

#include "Utilities/FileHandler.h"
#include "Core/Base.h"
#include "Core/Rendering/Mesh.h"
#include "Core/Rendering/ITexture.h"
#include "Core/Rendering/Material.h"
#include "Core/Asset/ModelImporter.h"
#include "Core/Asset/MaterialSerializer.h"
#include "Core/Asset/TextureImporter.h"
#include "Core/Asset/AssetMetaData.h"

namespace Dwarf {
    /// @brief Component containing a model asset.
    struct MeshAsset {
        /// @brief Vector of submeshes.
        std::vector<Mesh> m_Meshes;

        MeshAsset(std::filesystem::path path) : m_Meshes(ModelImporter::Import(path)){ }
    };

    /// @brief Component containing a material asset.
    struct MaterialAsset {
        /// @brief Imported material.
        Ref<Material> m_Material;

        MaterialAsset(std::filesystem::path path) {
            // Use Mesh Importer with meta data to import mesh
            m_Material = MaterialSerializer::Deserialize(path);
        }
    };

    /// @brief Component containing a vertex shader asset.
    struct VertexShaderAsset {
        /// @brief Path to the asset.
        std::filesystem::path m_Path;
        std::string m_FileContent;

        VertexShaderAsset(std::filesystem::path path) : m_Path(path), m_FileContent(FileHandler::ReadFile(m_Path)) {}
    };

    /// @brief Component containing a fragment shader asset.
    struct FragmentShaderAsset {
        /// @brief Path to the asset.
        std::filesystem::path m_Path;
        std::string m_FileContent;

        FragmentShaderAsset(std::filesystem::path path) : m_Path(path), m_FileContent(FileHandler::ReadFile(m_Path)) {}
    };

    /// @brief Component containing a geometry shader asset.
    struct GeometryShaderAsset {
        /// @brief Path to the asset.
        std::filesystem::path m_Path;
        std::string m_FileContent;

        GeometryShaderAsset(std::filesystem::path path) : m_Path(path), m_FileContent(FileHandler::ReadFile(m_Path)) {}
    };

    /// @brief Component containing a tesselation control shader asset.
    struct TesselationControlShaderAsset {
        /// @brief Path to the asset.
        std::filesystem::path m_Path;
        std::string m_FileContent;

        TesselationControlShaderAsset(std::filesystem::path path) : m_Path(path), m_FileContent(FileHandler::ReadFile(m_Path)) {}
    };

    /// @brief Component containing a tesselation evaluation shader asset.
    struct TesselationEvaluationShaderAsset {
        /// @brief Path to the asset.
        std::filesystem::path m_Path;
        std::string m_FileContent;

        TesselationEvaluationShaderAsset(std::filesystem::path path) : m_Path(path), m_FileContent(FileHandler::ReadFile(m_Path)) {}
    };

    /// @brief Component containing a compute shader asset.
    struct ComputeShaderAsset {
        /// @brief Path to the asset.
        std::filesystem::path m_Path;
        std::string m_FileContent;

        ComputeShaderAsset(std::filesystem::path path) : m_Path(path), m_FileContent(FileHandler::ReadFile(m_Path)) {}
    };

    /// @brief Component containing a compute shader asset.
    struct MetalShaderAsset {
        /// @brief Path to the asset.
        std::filesystem::path m_Path;
        std::string m_FileContent;

        MetalShaderAsset(std::filesystem::path path) : m_Path(path), m_FileContent(FileHandler::ReadFile(m_Path)) {}
    };

    /// @brief Component containing a compute shader asset.
    struct HlslShaderAsset {
        /// @brief Path to the asset.
        std::filesystem::path m_Path;
        std::string m_FileContent;

        HlslShaderAsset(std::filesystem::path path) : m_Path(path), m_FileContent(FileHandler::ReadFile(m_Path)) {}
    };

    /// @brief Component containing a texture asset.
    struct TextureAsset {
        /// @brief Imported texture.
        Ref<ITexture> m_Texture;

        TextureAsset(std::filesystem::path path) : m_Texture(TextureImporter::CreateTexture(path)) { }

        /// @brief Retrieves the texture.
        /// @return The imported texture instance.
        Ref<ITexture> Get() {
            return m_Texture;
        }
    };

    /// @brief Asset containing a Dwarf Engine scene.
    struct SceneAsset {
        /// @brief Path to the asset.
        std::filesystem::path m_Path;

        SceneAsset(std::filesystem::path path) : m_Path(path) {}
    };

    struct UnknownAsset {
        std::filesystem::path m_Path;
        std::string m_FileContent;
        UnknownAsset(std::filesystem::path path) : m_Path(path), m_FileContent(FileHandler::ReadFile(m_Path)) {}
    };
}