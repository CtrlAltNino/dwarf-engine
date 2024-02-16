#include "dpch.h"
#include "Core/Asset/MaterialSerializer.h"

#ifdef _WIN32
// #include "Platform/Direct3D12/D3D12Shader.h"
#include "Platform/OpenGL/OpenGLShader.h"
// #include "Platform/Vulkan/VulkanShader.h"
#elif __linux__
#include "Platform/OpenGL/OpenGLShader.h"
// #include "Platform/Vulkan/VulkanShader.h"
#elif __APPLE__
// #include "Platform/Metal/MetalShader.h"
#endif
#include <Core/Rendering/Shader Parameters/BooleanShaderParameter.h>
#include <Core/Rendering/Shader Parameters/IntegerShaderParameter.h>
#include <Core/Rendering/Shader Parameters/FloatShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Tex2DShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec2ShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec3ShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec4ShaderParameter.h>

namespace Dwarf
{
    Ref<Material> MaterialSerializer::Deserialize(std::filesystem::path path)
    {
        Material deserializedMat(path.stem().string());

        nlohmann::json serializedMat = nlohmann::json::parse(FileHandler::ReadFile(path));

        deserializedMat.SetTransparency((bool)serializedMat["transparent"]);

        if (serializedMat.contains("parameters"))
        {
            for (auto parameter : serializedMat["parameters"].items())
            {

                if (serializedMat["parameters"][parameter.key()]["type"] == "boolean")
                {
                    deserializedMat.SetParameter(parameter.key(), bool(parameter.value()["value"]));
                }
                else if (serializedMat["parameters"][parameter.key()]["type"] == "integer")
                {
                    deserializedMat.SetParameter(parameter.key(), int(parameter.value()["value"]));
                }
                else if (serializedMat["parameters"][parameter.key()]["type"] == "float")
                {
                    deserializedMat.SetParameter(parameter.key(), float(parameter.value()["value"]));
                }
                else if (serializedMat["parameters"][parameter.key()]["type"] == "tex2d")
                {
                    if (int(parameter.value()["value"]) != -1)
                    {
                        deserializedMat.SetParameter(parameter.key(), CreateRef<UID>(UID(parameter.value()["value"])), TEX2D);
                    }
                    else
                    {
                        deserializedMat.SetParameter(parameter.key(), nullptr, TEX2D);
                    }
                }
                else if (serializedMat["parameters"][parameter.key()]["type"] == "vec2")
                {
                    deserializedMat.SetParameter(parameter.key(), glm::vec2((float)parameter.value()["value"]["x"],
                                                                            (float)parameter.value()["value"]["y"]));
                }
                else if (serializedMat["parameters"][parameter.key()]["type"] == "vec3")
                {
                    deserializedMat.SetParameter(parameter.key(), glm::vec3((float)parameter.value()["value"]["x"],
                                                                            (float)parameter.value()["value"]["y"],
                                                                            (float)parameter.value()["value"]["z"]));
                }
                else if (serializedMat["parameters"][parameter.key()]["type"] == "vec4")
                {
                    deserializedMat.SetParameter(parameter.key(), glm::vec4((float)parameter.value()["value"]["x"],
                                                                            (float)parameter.value()["value"]["y"],
                                                                            (float)parameter.value()["value"]["z"],
                                                                            (float)parameter.value()["value"]["w"]));
                }
            }
        }

        if (serializedMat.contains("shader") && serializedMat["shader"] != "default")
        {
            switch (Renderer::GetAPI())
            {
#ifdef _WIN32
            case GraphicsApi::D3D12:
                break;
            case GraphicsApi::Metal:
                break;
            case GraphicsApi::OpenGL:
            {
                Ref<OpenGLShader> shader = std::dynamic_pointer_cast<OpenGLShader>(Shader::Create());

                if (serializedMat["shader"].contains("vertexShader") && serializedMat["shader"]["vertexShader"] != "default")
                {
                    shader->SetVertexShader(CreateRef<UID>(UID(serializedMat["shader"]["vertexShader"])));
                }
                else
                {
                    shader->SetVertexShader(FileHandler::ReadFile(Shader::GetDefaultShaderPath() / "default.vert").c_str());
                }

                if (serializedMat["shader"].contains("fragmentShader") && serializedMat["shader"]["fragmentShader"] != "default")
                {
                    shader->SetFragmentShader(CreateRef<UID>(UID(serializedMat["shader"]["fragmentShader"])));
                }
                else
                {
                    shader->SetFragmentShader(FileHandler::ReadFile(Shader::GetDefaultShaderPath() / "default.frag").c_str());
                }

                if (serializedMat["shader"].contains("geometryShader"))
                {
                    shader->SetGeometryShader(CreateRef<UID>(UID(serializedMat["shader"]["geometryShader"])));
                }
                deserializedMat.SetShader(shader);
                break;
            }
            case GraphicsApi::Vulkan:
                break;
#elif __linux__
            case GraphicsApi::D3D12:
                break;
            case GraphicsApi::Metal:
                break;
            case GraphicsApi::OpenGL:
            {
                Ref<OpenGLShader> shader = std::dynamic_pointer_cast<OpenGLShader>(Shader::Create());

                if (serializedMat["shader"].contains("vertexShader") && serializedMat["shader"]["vertexShader"] != "default")
                {
                    shader->SetVertexShader(CreateRef<UID>(UID(serializedMat["shader"]["vertexShader"])));
                }
                else
                {
                    shader->SetVertexShader(FileHandler::ReadFile(Shader::GetDefaultShaderPath() / "default.vert").c_str());
                }

                if (serializedMat["shader"].contains("fragmentShader") && serializedMat["shader"]["fragmentShader"] != "default")
                {
                    shader->SetFragmentShader(CreateRef<UID>(UID(serializedMat["shader"]["fragmentShader"])));
                }
                else
                {
                    shader->SetFragmentShader(FileHandler::ReadFile(Shader::GetDefaultShaderPath() / "default.frag").c_str());
                }

                if (serializedMat["shader"].contains("geometryShader"))
                {
                    shader->SetGeometryShader(CreateRef<UID>(UID(serializedMat["shader"]["geometryShader"])));
                }
                deserializedMat.SetShader(shader);
                break;
            }
            case GraphicsApi::Vulkan:
                break;
#elif __APPLE__
            case GraphicsApi::D3D12:
                break;
            case GraphicsApi::Metal:
                break;
            case GraphicsApi::OpenGL:
                break;
            case GraphicsApi::Vulkan:
                break;
#endif
            }
        }
        return CreateRef<Material>(deserializedMat);
    }

    void MaterialSerializer::Serialize(Material material, std::filesystem::path path)
    {
        nlohmann::json serializedMat;

        serializedMat["transparent"] = material.IsTransparent();

        switch (Renderer::GetAPI())
        {
#ifdef _WIN32
        case GraphicsApi::D3D12:
            break;
        case GraphicsApi::Metal:
            break;
        case GraphicsApi::OpenGL:
        {
            Ref<Shader> shaderRef = material.GetShader();
            OpenGLShader *shader = (OpenGLShader *)shaderRef.get();

            if (shader->GetVertexShader() != nullptr)
            {
                serializedMat["shader"]["vertexShader"] = (uint64_t)*shader->GetVertexShader();
            }
            else
            {
                serializedMat["shader"]["vertexShader"] = "default";
            }

            if (shader->GetFragmentShader() != nullptr)
            {
                serializedMat["shader"]["fragmentShader"] = (uint64_t)*shader->GetFragmentShader();
            }
            else
            {
                serializedMat["shader"]["fragmentShader"] = "default";
            }
            if (serializedMat["shader"].contains("geometryShader"))
            {
                serializedMat["shader"]["geometryShader"] = (uint64_t)*shader->GetGeometryShader();
            }
            break;
        }
        case GraphicsApi::Vulkan:
            break;
#elif __linux__
        case GraphicsApi::D3D12:
            break;
        case GraphicsApi::Metal:
            break;
        case GraphicsApi::OpenGL:
        {
            Ref<Shader> shaderRef = material.GetShader();
            OpenGLShader *shader = (OpenGLShader *)shaderRef.get();
            serializedMat["shader"]["vertexShader"] = (uint64_t)*shader->GetVertexShader();
            serializedMat["shader"]["fragmentShader"] = (uint64_t)*shader->GetFragmentShader();
            if (serializedMat["shader"].contains("geometryShader"))
            {
                serializedMat["shader"]["geometryShader"] = (uint64_t)*shader->GetGeometryShader();
            }
            break;
        }
        case GraphicsApi::Vulkan:
            break;
#elif __APPLE__
        case GraphicsApi::D3D12:
            break;
        case GraphicsApi::Metal:
            break;
        case GraphicsApi::OpenGL:
            break;
        case GraphicsApi::Vulkan:
            break;
#endif
        }

        for (auto const &[key, val] : material.m_Parameters)
        {
            switch ((*val).GetType())
            {
            case BOOLEAN:
                serializedMat["parameters"][key]["type"] = "boolean";
                serializedMat["parameters"][key]["value"] = std::dynamic_pointer_cast<BooleanShaderParameter>(val)->m_Value;
                break;
            case INTEGER:
                serializedMat["parameters"][key]["type"] = "integer";
                serializedMat["parameters"][key]["value"] = std::dynamic_pointer_cast<IntegerShaderParameter>(val)->m_Value;
                break;
            case FLOAT:
                serializedMat["parameters"][key]["type"] = "float";
                serializedMat["parameters"][key]["value"] = std::dynamic_pointer_cast<FloatShaderParameter>(val)->m_Value;
                break;
            case TEX2D:
                serializedMat["parameters"][key]["type"] = "tex2d";
                if (std::dynamic_pointer_cast<Tex2DShaderParameter>(val)->m_Value != nullptr)
                {
                    serializedMat["parameters"][key]["value"] = (uint64_t)*std::dynamic_pointer_cast<Tex2DShaderParameter>(val)->m_Value;
                }
                else
                {
                    serializedMat["parameters"][key]["value"] = -1;
                }
                break;
            case VEC2:
                serializedMat["parameters"][key]["type"] = "vec2";
                serializedMat["parameters"][key]["value"]["x"] = std::dynamic_pointer_cast<Vec2ShaderParameter>(val)->m_Value.x;
                serializedMat["parameters"][key]["value"]["y"] = std::dynamic_pointer_cast<Vec2ShaderParameter>(val)->m_Value.y;
                break;
            case VEC3:
                serializedMat["parameters"][key]["type"] = "vec3";
                serializedMat["parameters"][key]["value"]["x"] = std::dynamic_pointer_cast<Vec3ShaderParameter>(val)->m_Value.x;
                serializedMat["parameters"][key]["value"]["y"] = std::dynamic_pointer_cast<Vec3ShaderParameter>(val)->m_Value.y;
                serializedMat["parameters"][key]["value"]["z"] = std::dynamic_pointer_cast<Vec3ShaderParameter>(val)->m_Value.z;
                break;
            case VEC4:
                serializedMat["parameters"][key]["type"] = "vec4";
                serializedMat["parameters"][key]["value"]["x"] = std::dynamic_pointer_cast<Vec4ShaderParameter>(val)->m_Value.x;
                serializedMat["parameters"][key]["value"]["y"] = std::dynamic_pointer_cast<Vec4ShaderParameter>(val)->m_Value.y;
                serializedMat["parameters"][key]["value"]["z"] = std::dynamic_pointer_cast<Vec4ShaderParameter>(val)->m_Value.z;
                serializedMat["parameters"][key]["value"]["w"] = std::dynamic_pointer_cast<Vec4ShaderParameter>(val)->m_Value.w;
                break;
            }
        }

        FileHandler::WriteToFile(path, serializedMat.dump(4));
    }
}