#pragma once
#include <nlohmann/json.hpp>
#include "../../utilities/dpch.h"
#include "../data structures/Mesh.h"
#include "../data structures/Texture.h"
#include "../rendering/Material.h"
#include "../../utilities/FileHandler.h"
#include "AssetMetaDataHelper.h"
#include "asset loader/ModelImporter.h"
#include "asset loader/MaterialImporter.h"

struct PathComponent {
    std::filesystem::path path;

    PathComponent(std::filesystem::path path) : path(path) {}
};

struct MeshAsset {
    std::vector<Mesh> meshes;
    std::filesystem::path path;
    boolean loaded;

    MeshAsset(std::filesystem::path path) : path(path){}

    // Loads the mesh from the asset path
    void Load(){
        // Use Mesh Importer with meta data to import mesh
        meshes = ModelImporter::Import(this->path, AssetMetaDataHelper::GetMetaData(this->path));
        loaded = true;
    }

    void Unload() {
        // TODO
        loaded = false;
    }

    // Retrieves the mesh
    std::vector<Mesh>* Get(){
        return &meshes;
    }
};

struct MaterialAsset {
    Material material;
    std::filesystem::path path;
    boolean loaded;

    MaterialAsset(std::filesystem::path path) : path(path) { }

    // Loads the material at the path
    void Load(){
        // Use Mesh Importer with meta data to import mesh
        material = MaterialImporter::Import(this->path, AssetMetaDataHelper::GetMetaData(this->path));
    }

    // Loads the material at the path
    void Unload(){
        // TODO
    }

    Material* Get(){
        return &material;
    }
};

struct VertexShaderAsset {
    std::filesystem::path path;

    VertexShaderAsset(std::filesystem::path path) : path(path) {}
};

struct FragmentShaderAsset {
    std::filesystem::path path;

    FragmentShaderAsset(std::filesystem::path path) : path(path) {}
};

struct GeometryShaderAsset {
    std::filesystem::path path;

    GeometryShaderAsset(std::filesystem::path path) : path(path) {}
};

struct TesselationControlShaderAsset {
    std::filesystem::path path;

    TesselationControlShaderAsset(std::filesystem::path path) : path(path) {}
};

struct TesselationEvaluationShaderAsset {
    std::filesystem::path path;

    TesselationEvaluationShaderAsset(std::filesystem::path pathh) : path(path) {}
};

struct ComputeShaderAsset {
    std::filesystem::path path;

    ComputeShaderAsset(std::filesystem::path path) : path(path) {}
};

struct TextureAsset {
    Texture texture;
    std::filesystem::path path;
    boolean loaded;

    TextureAsset(std::filesystem::path path) : path(path) {}

    void Load() {
        loaded = true;
    }

    void Unload() {
        loaded = false;
    }

    Texture* Get() {
        return &texture;
    }
};

struct SceneAsset {
    std::filesystem::path path;

    SceneAsset(std::filesystem::path path) : path(path) {}
};