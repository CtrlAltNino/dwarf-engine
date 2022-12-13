#pragma once
#include <nlohmann/json.hpp>
#include "../../utilities/dpch.h"
#include "../data structures/Mesh.h"
#include "../data structures/Texture.h"
#include "../rendering/Material.h"
#include "../../utilities/FileHandler.h"
#include "AssetMetaDataUtilities.h"
#include "asset loader/ModelUtilities.h"
#include "asset loader/MaterialUtilities.h"

/// @brief Component containing the path of the asset.
struct PathComponent {
    /// @brief Path to an asset.
    std::filesystem::path path;

    PathComponent(std::filesystem::path path) : path(path) {}
};

/// @brief Component containing a model asset.
struct MeshAsset {
    /// @brief Vector of submeshes.
    std::vector<Mesh> meshes;

    /// @brief Path to the model.
    std::filesystem::path path;

    /// @brief Flag indicating if the model is loaded.
    boolean loaded;

    MeshAsset(std::filesystem::path path) : path(path){}

    /// @brief Loads the model into memory.
    void Load(){
        // Use Mesh Importer with meta data to import mesh
        meshes = ModelUtilities::Import(this->path);
        loaded = true;
    }

    /// @brief Unloads the model.
    void Unload() {
        // TODO
        loaded = false;
    }

    /// @brief Retrieves the model.
    /// @return List of meshes of the imported model.
    std::vector<Mesh>* Get(){
        return &meshes;
    }
};

/// @brief Component containing a material asset.
struct MaterialAsset {
    /// @brief Imported material.
    Material material;

    /// @brief Path to the material asset.
    std::filesystem::path path;

    /// @brief Flag indicating if the model is loaded.
    boolean loaded;

    MaterialAsset(std::filesystem::path path) : path(path) { }

    /// @brief Loads the material into memory
    void Load(){
        // Use Mesh Importer with meta data to import mesh
        material = MaterialUtilities::Import(this->path);
    }

    /// @brief Unloads the material.
    void Unload(){
        // TODO
    }

    /// @brief Retrieves the material.
    /// @return The imported material instance.
    Material* Get(){
        return &material;
    }
};

/// @brief Component containing a vertex shader asset.
struct VertexShaderAsset {
    /// @brief Path to the asset.
    std::filesystem::path path;

    VertexShaderAsset(std::filesystem::path path) : path(path) {}
};

/// @brief Component containing a fragment shader asset.
struct FragmentShaderAsset {
    /// @brief Path to the asset.
    std::filesystem::path path;

    FragmentShaderAsset(std::filesystem::path path) : path(path) {}
};

/// @brief Component containing a geometry shader asset.
struct GeometryShaderAsset {
    /// @brief Path to the asset.
    std::filesystem::path path;

    GeometryShaderAsset(std::filesystem::path path) : path(path) {}
};

/// @brief Component containing a tesselation control shader asset.
struct TesselationControlShaderAsset {
    /// @brief Path to the asset.
    std::filesystem::path path;

    TesselationControlShaderAsset(std::filesystem::path path) : path(path) {}
};

/// @brief Component containing a tesselation evaluation shader asset.
struct TesselationEvaluationShaderAsset {
    /// @brief Path to the asset.
    std::filesystem::path path;

    TesselationEvaluationShaderAsset(std::filesystem::path pathh) : path(path) {}
};

/// @brief Component containing a compute shader asset.
struct ComputeShaderAsset {
    /// @brief Path to the asset.
    std::filesystem::path path;

    ComputeShaderAsset(std::filesystem::path path) : path(path) {}
};

/// @brief Component containing a texture asset.
struct TextureAsset {
    /// @brief Imported texture.
    Texture texture;
    
    /// @brief Path to the asset.
    std::filesystem::path path;

    /// @brief Flag indicating if the model is loaded.
    boolean loaded;

    TextureAsset(std::filesystem::path path) : path(path) {}

    /// @brief Imports and loads the texture into memory.
    void Load() {
        loaded = true;
    }

    /// @brief Unloads the texture.
    void Unload() {
        loaded = false;
    }

    /// @brief Retrieves the texture.
    /// @return The imported texture instance.
    Texture* Get() {
        return &texture;
    }
};

/// @brief Asset containing a Dwarf Engine scene.
struct SceneAsset {
    /// @brief Path to the asset.
    std::filesystem::path path;

    SceneAsset(std::filesystem::path path) : path(path) {}
};