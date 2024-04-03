#pragma once

#include "Core/Rendering/ComputeShader.h"
#include "Core/Base.h"

#include <string_view>

#include <glad/glad.h>
#include <Core/Rendering/IShaderParameter.h>

namespace Dwarf
{
    class OpenGLComputeShader : public ComputeShader
    {
    private:
        GLuint m_ID = -1;
        std::string m_ComputeShaderLog;
        std::string m_ComputeShaderSource;
        Ref<UID> m_ComputeShaderAsset;

    public:
        OpenGLComputeShader();
        ~OpenGLComputeShader() override;

        void SetComputeShader(Ref<UID> computeShader);
        void SetComputeShader(std::string_view computeShader);
        std::map<std::string, Ref<IShaderParameter>, std::less<>> GetParameters() override;
        void UpdateParameters() override;

        Ref<UID> GetComputeShader() const;

        GLuint GetID() const;

        void Compile() override;

        static Ref<OpenGLComputeShader> CreatePropagationShader();
        static Ref<OpenGLComputeShader> CreateFinalizationShader();

        const std::string &GetLog() const;
        Ref<UID> &GetAsset();
    };
}