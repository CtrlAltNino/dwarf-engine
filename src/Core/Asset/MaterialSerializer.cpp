#include "Core/Asset/MaterialSerializer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Dwarf {
    Ref<Material> MaterialSerializer::Deserialize(std::filesystem::path path){
        Material deserializedMat(path.stem().string());

        nlohmann::json serializedMat = nlohmann::json::parse(FileHandler::ReadFile(path));
        
        if(serializedMat.contains("shader") && serializedMat["shader"] != "default"){
            switch(Renderer::GetAPI()){
#ifdef _WIN32
                case GraphicsApi::D3D11: break;
                case GraphicsApi::D3D12: break;
                case GraphicsApi::Metal: break;
                case GraphicsApi::OpenGL:
                        Ref<Shader> shaderRef = Shader::Create();
                        OpenGLShader* shader = (OpenGLShader*)shaderRef.get();
                        shader->SetVertexShader(CreateRef<UID>(UID(serializedMat["shader"]["vertexShader"])));
                        shader->SetFragmentShader(CreateRef<UID>(UID(serializedMat["shader"]["fragmentShader"])));
                        if(serializedMat["shader"].contains("geometryShader")){
                            shader->SetGeometryShader(CreateRef<UID>(UID(serializedMat["shader"]["geometryShader"])));
                        }
                        deserializedMat.SetShader(shaderRef);
                    break;
#elif __linux__

#elif __APPLE__

#endif
            }
        }
        return CreateRef<Material>(deserializedMat);
    }

    void MaterialSerializer::Serialize(Material material, std::filesystem::path path) {
        nlohmann::json serializedMat;

        serializedMat["transparent"] = material.IsTransparent();
        
        switch(Renderer::GetAPI()){
#ifdef _WIN32
                case GraphicsApi::D3D11: break;
                case GraphicsApi::D3D12: break;
                case GraphicsApi::Metal: break;
                case GraphicsApi::OpenGL:
                        Ref<Shader> shaderRef = material.GetShader();
                        OpenGLShader* shader = (OpenGLShader*)shaderRef.get();
                        serializedMat["shader"]["vertexShader"] = (uint64_t)*shader->GetVertexShader();
                        serializedMat["shader"]["fragmentShader"] = (uint64_t)*shader->GetFragmentShader();
                        if(serializedMat["shader"].contains("geometryShader")){
                            serializedMat["shader"]["geometryShader"] = (uint64_t)*shader->GetGeometryShader();
                        }
                    break;
#elif __linux__

#elif __APPLE__

#endif
        }

        for(auto const& [key, val] : material.GetTextures()){
            serializedMat["textures"][key] = (uint64_t)*val;
        }

        for(auto const& [key, val] : material.GetBoolUniforms()){
            serializedMat["boolUniforms"][key] = val;
        }

        for(auto const& [key, val] : material.GetIntegerUniforms()){
            serializedMat["integerUniforms"][key] = val;
        }

        for(auto const& [key, val] : material.GetFloatUniforms()){
            serializedMat["floatUniforms"][key] = val;
        }

        for(auto const& [key, val] : material.Get2DUniforms()){
            serializedMat["vec2Uniforms"][key]["x"] = val.x;
            serializedMat["vec2Uniforms"][key]["y"] = val.y;
        }

        for(auto const& [key, val] : material.Get3DUniforms()){
            serializedMat["vec3Uniforms"][key]["x"] = val.x;
            serializedMat["vec3Uniforms"][key]["y"] = val.y;
            serializedMat["vec3Uniforms"][key]["z"] = val.z;
        }

        for(auto const& [key, val] : material.Get4DUniforms()){
            serializedMat["vec4Uniforms"][key]["x"] = val.x;
            serializedMat["vec4Uniforms"][key]["y"] = val.y;
            serializedMat["vec4Uniforms"][key]["z"] = val.z;
            serializedMat["vec4Uniforms"][key]["w"] = val.w;
        }

        FileHandler::WriteToFile(path, serializedMat.dump(4));
    }
}