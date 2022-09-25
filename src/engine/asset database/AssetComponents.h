#pragma once
#include <nlohmann/json.hpp>
#include "../../utilities/dpch.h"
#include "../data structures/Mesh.h"
#include "../data structures/Texture.h"
#include "../rendering/Material.h"
#include "../rendering/Shader.h"
#include "../../utilities/FileHandler.h"

struct Path {
    std::string path;
};

struct MeshAsset {
    Mesh mesh;

    void LoadMesh(std::string path){
        // Get Meta data if present
        nlohmann::json json;
        if(FileHandler::checkIfFileExists(path + ".meta")){
            std::string fileContent = FileHandler::readFile(projectSettingsPath.c_str());
            
            ProjectInformation foundInfo;

            if (!fileContent.empty()) {
                json = nlohmann::json::parse(fileContent);
            }
        }
        // Import mesh with assimp

        // json.contains("projectInformation")

        
    }

    nlohmann::json GetMetaData(std::string path){
        nlohmann::json json;
        
        if(FileHandler::checkIfFileExists(path + ".meta")){
            std::string fileContent = FileHandler::readFile(projectSettingsPath.c_str());
            
            ProjectInformation foundInfo;

            if (!fileContent.empty()) {
                json = nlohmann::json::parse(fileContent);
            }
        }

        return json;
    }

    void SetMetaData(std::string directory, std::string assetName, nlohmann::json metaData){
        std::string fileContent = metaData.dump(4);
        if (!FileHandler::checkIfDirectoyExists(directory)) {
            FileHandler::createDirectoryS(directory);
        }
        std::string savedProjectsPath = settingsPath + directory + ".meta";
        FileHandler::writeToFile(savedProjectsPath.c_str(), fileContent);
    }

    Mesh* GetMesh(){
        return &mesh;
    }
};

struct MaterialAsset {
    Material material;

    void LoadMaterial(std::string path){
        // Get Meta data if present
        // 
    }

    nlohmann::json GetMetaData(std::string path){
        
    }

    void SetMetaData(std::string path, nlohmann::json metaData){
        
    }

    Material* GetMaterial(){
        return &material;
    }
};

struct ShaderAsset {
    Shader shader;

    void LoadShader(std::string path){

    }

    nlohmann::json GetMetaData(std::string path){
        
    }

    void SetMetaData(std::string path, nlohmann::json metaData){
        
    }

    Shader* GetShader(){
        return &shader;
    }
};

struct TextureAsset {
    Texture texture;

    void LoadTexture(std::string path){

    }

    nlohmann::json GetMetaData(std::string path){
        
    }

    void SetMetaData(std::string path, nlohmann::json metaData){
        
    }

    Texture* GetTexture(){
        return &texture;
    }
};

struct SceneAsset {

};