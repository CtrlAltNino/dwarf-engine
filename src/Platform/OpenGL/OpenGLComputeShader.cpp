#include "dpch.h"

#include <utility>

#include "OpenGLComputeShader.h"
#include "Utilities/FileHandler.h"
#include "Core/Asset/AssetDatabase.h"
#include "Core/Asset/AssetComponents.h"
#include <Core/Rendering/Shader Parameters/Tex2DShaderParameter.h>
#include <Core/Rendering/Shader Parameters/BooleanShaderParameter.h>
#include <Core/Rendering/Shader Parameters/IntegerShaderParameter.h>
#include <Core/Rendering/Shader Parameters/UnsignedIntegerShaderParameter.h>
#include <Core/Rendering/Shader Parameters/FloatShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec4ShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec2ShaderParameter.h>
#include <Core/Rendering/Shader Parameters/Vec3ShaderParameter.h>

#define GL_SHADER_LOG_LENGTH (1024)

namespace Dwarf
{
    OpenGLComputeShader::OpenGLComputeShader() = default;
    OpenGLComputeShader::~OpenGLComputeShader() = default;

    const std::map<GLenum, ShaderParameterType> glTypeToDwarfShaderType =
        {
            {GL_BOOL, ShaderParameterType::BOOLEAN},
            {GL_INT, ShaderParameterType::INTEGER},
            {GL_FLOAT, ShaderParameterType::FLOAT},
            {GL_FLOAT_VEC2, ShaderParameterType::VEC2},
            {GL_FLOAT_VEC3, ShaderParameterType::VEC3},
            {GL_FLOAT_VEC4, ShaderParameterType::VEC4},
            {GL_SAMPLER_2D, ShaderParameterType::TEX2D}};

    void OpenGLComputeShader::Compile()
    {
        SetIsCompiled(false);

        std::string shaderSource;

        if (m_ComputeShaderAsset != nullptr && AssetDatabase::Exists(m_ComputeShaderAsset))
        {
            std::filesystem::path shaderPath = AssetDatabase::Retrieve<ComputeShaderAsset>(m_ComputeShaderAsset)->GetAsset()->m_Path;
            shaderSource = FileHandler::ReadFile(shaderPath);
            // TODO: This needs to move somewhere else
            // AssetDatabase::AddShaderWatch(shaderPath, CreateRef<ComputeShader>(*this));
        }
        else
        {
            shaderSource = m_ComputeShaderSource;
        }

        if (shaderSource.length() > 0)
        {
            const char *shadercstr = shaderSource.c_str();

            GLsizei log_length = 0;
            GLchar message[1024] = "";

            GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
            glShaderSource(shader, 1, &shadercstr, nullptr);
            glCompileShader(shader);

            GLint compiled;
            glGetShaderInfoLog(shader, 1024, &log_length, message);
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

            if (log_length > 0)
            {
                m_ComputeShaderLog = std::string(message);
                std::cout << "Initial shader log: " << m_ComputeShaderLog << "\n";
            }
            if (compiled != GL_TRUE)
            {
                glDeleteShader(shader);
                return;
            }

            m_ID = glCreateProgram();

            glAttachShader(m_ID, shader);

            glLinkProgram(m_ID);

            glDeleteShader(shader);

            SetIsCompiled(true);

            SetParameters(GetParameters());
        }
        else
        {
            // TODO: log missing shader error
        }
    }

    void OpenGLComputeShader::SetComputeShader(Ref<UID> computeShader)
    {
        m_ComputeShaderAsset = computeShader;
        m_ComputeShaderSource = "";
    }

    void OpenGLComputeShader::SetComputeShader(std::string_view computeShader)
    {
        m_ComputeShaderAsset = nullptr;
        m_ComputeShaderSource = computeShader;
    }

    GLuint OpenGLComputeShader::GetID() const
    {
        return m_ID;
    }

    Ref<OpenGLComputeShader> OpenGLComputeShader::CreatePropagationShader()
    {
        Ref<OpenGLComputeShader> compShader = CreateRef<OpenGLComputeShader>(OpenGLComputeShader());
        compShader->SetComputeShader(FileHandler::ReadFile(ComputeShader::GetOutlineShaderPath() / "propagation.comp"));
        compShader->Compile();
        return compShader;
    }

    Ref<OpenGLComputeShader> OpenGLComputeShader::CreateFinalizationShader()
    {
        Ref<OpenGLComputeShader> compShader = CreateRef<OpenGLComputeShader>(OpenGLComputeShader());
        compShader->SetComputeShader(FileHandler::ReadFile(ComputeShader::GetOutlineShaderPath() / "finalization.comp"));
        compShader->Compile();
        return compShader;
    }

    std::map<std::string, Ref<IShaderParameter>, std::less<>> OpenGLComputeShader::GetParameters()
    {
        auto parameters = std::map<std::string, Ref<IShaderParameter>, std::less<>>();
        GLint i;
        GLint count;

        GLint size;  // size of the variable
        GLenum type; // type of the variable (float, vec3 or mat4, etc)

        const GLsizei bufSize = 64; // maximum name length
        GLchar name[bufSize];       // variable name in GLSL
        GLsizei length;             // name length

        glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &count);

        for (i = 0; i < count; i++)
        {
            glGetActiveUniform(m_ID, (GLuint)i, bufSize, &length, &size, &type, name);

            parameters[std::string(name)] = ComputeShader::CreateComputeShaderParameter(glTypeToDwarfShaderType.find(type)->second);
        }

        return parameters;
    }

    void OpenGLComputeShader::UpdateParameters()
    {
        for (auto const &[key, val] : GetParameters())
        {
            switch (val->GetType())
            {
            case ShaderParameterType::BOOLEAN:
            {
                bool value = std::dynamic_pointer_cast<BooleanShaderParameter>(val)->m_Value;
                glUniform1i(glGetUniformLocation(m_ID, key.c_str()), value);
            }
            break;
            case ShaderParameterType::INTEGER:
            {
                int value = std::dynamic_pointer_cast<IntegerShaderParameter>(val)->m_Value;
                glUniform1i(glGetUniformLocation(m_ID, key.c_str()), value);
            }
            break;
            case ShaderParameterType::UNSIGNED_INTEGER:
            {
                int value = std::dynamic_pointer_cast<UnsignedIntegerShaderParameter>(val)->m_Value;
                glUniform1ui(glGetUniformLocation(m_ID, key.c_str()), value);
            }
            break;
            case ShaderParameterType::FLOAT:
            {
                float value = std::dynamic_pointer_cast<FloatShaderParameter>(val)->m_Value;
                glUniform1f(glGetUniformLocation(m_ID, key.c_str()), value);
            }
            break;
            case ShaderParameterType::VEC2:
            {
                glm::vec2 value = std::dynamic_pointer_cast<Vec2ShaderParameter>(val)->m_Value;
                glUniform2f(glGetUniformLocation(m_ID, key.c_str()), value.x, value.y);
            }
            break;
            case ShaderParameterType::VEC3:
            {
                glm::vec3 value = std::dynamic_pointer_cast<Vec3ShaderParameter>(val)->m_Value;
                glUniform3f(glGetUniformLocation(m_ID, key.c_str()), value.x, value.y, value.z);
            }
            break;
            case ShaderParameterType::VEC4:
            {
                glm::vec4 value = std::dynamic_pointer_cast<Vec4ShaderParameter>(val)->m_Value;
                glUniform4f(glGetUniformLocation(m_ID, key.c_str()), value.x, value.y, value.z, value.w);
            }
            break;
            case ShaderParameterType::TEX2D:
            {
                Ref<UID> parameter = std::dynamic_pointer_cast<Tex2DShaderParameter>(val)->m_Value;
                if (parameter)
                {
                    glActiveTexture(GL_TEXTURE0 + 0);
                    glBindTexture(GL_TEXTURE_2D, AssetDatabase::Retrieve<TextureAsset>(parameter)->GetAsset()->m_Texture->GetTextureID());

                    GLuint uniformID = glGetUniformLocation(m_ID, key.c_str());
                    glUniform1i(uniformID, 0);
                }
            }
            break;
            }
        }
    }

    const std::string &OpenGLComputeShader::GetLog() const
    {
        return m_ComputeShaderLog;
    }

    Ref<UID> &OpenGLComputeShader::GetAsset()
    {
        return m_ComputeShaderAsset;
    }
}