#pragma once

#include "Core/Rendering/Shader.h"
#include "Core/Base.h"

#include <glad/glad.h>
#include <Core/Rendering/IShaderParameter.h>

namespace Dwarf
{
    struct ShaderLogs
    {
        std::string m_VertexShaderLog;
        std::string m_TessellationControlShaderLog;
        std::string m_TessellationEvaluationShaderLog;
        std::string m_GeometryShaderLog;
        std::string m_FragmentShaderLog;
    };

    struct ShaderSources
    {
        std::string m_VertexShaderSource;
        std::string m_TessellationControlShaderSource;
        std::string m_TessellationEvaluationShaderSource;
        std::string m_GeometryShaderSource;
        std::string m_FragmentShaderSource;
    };

    struct ShaderAssets
    {
        Ref<UID> m_VertexShaderAsset;
        Ref<UID> m_TessellationControlShaderAsset;
        Ref<UID> m_TessellationEvaluationShaderAsset;
        Ref<UID> m_GeometryShaderAsset;
        Ref<UID> m_FragmentShaderAsset;
    };

    class OpenGLShader : public Shader
    {
    private:
        GLuint m_ID = -1;
        ShaderSources m_ShaderSources;
        ShaderAssets m_ShaderAssets;
        ShaderLogs m_ShaderLogs;

    public:
        OpenGLShader();
        ~OpenGLShader() override;

        void SetVertexShader(Ref<UID> vertexShader);
        void SetVertexShader(std::string const &vertexShader);

        void SetTesselaltionControlShader(Ref<UID> tessellationControlShader);
        void SetTesselaltionControlShader(std::string const &tessellationControlShader);

        void SetTesselaltionEvaluationShader(Ref<UID> tessellationEvaluationShader);
        void SetTesselaltionEvaluationShader(std::string const &tessellationEvaluationShader);

        void SetGeometryShader(Ref<UID> geometryShader);
        void SetGeometryShader(std::string const &geometryShader);

        void SetFragmentShader(Ref<UID> fragmentShader);
        void SetFragmentShader(std::string const &fragmentShader);

        Ref<UID> GetVertexShader() const;
        Ref<UID> GetFragmentShader() const;
        Ref<UID> GetTesselaltionControlShader() const;
        Ref<UID> GetTesselaltionEvaluationShader() const;
        Ref<UID> GetGeometryShader() const;

        GLuint GetID() const;

        void Compile() override;

        std::map<std::string, Ref<IShaderParameter>> GetParameters() override;

        static Ref<OpenGLShader> CreateDefaultShader();
        static Ref<OpenGLShader> CreateErrorShader();
        static Ref<OpenGLShader> CreateGridShader();
        static Ref<OpenGLShader> CreatePreviewShader();

        const ShaderLogs &GetShaderLogs() const;
        const ShaderAssets &GetShaderAssets() const;
    };
}