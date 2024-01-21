#pragma once

#include "Core/Rendering/Shader.h"
#include "Core/Base.h"

#include <glad/glad.h>
#include <Core/Rendering/IShaderParameter.h>

namespace Dwarf
{
    class OpenGLShader : public Shader
    {
    private:
        GLuint m_ID = -1;

    public:
        Ref<UID> m_VertexShaderAsset;
        Ref<UID> m_TessellationControlShaderAsset;
        Ref<UID> m_TessellationEvaluationShaderAsset;
        Ref<UID> m_GeometryShaderAsset;
        Ref<UID> m_FragmentShaderAsset;

        std::string m_VertexShaderSource;
        std::string m_TessellationControlShaderSource;
        std::string m_TessellationEvaluationShaderSource;
        std::string m_GeometryShaderSource;
        std::string m_FragmentShaderSource;

        GLsizei vert_log_length = 0;
        GLchar vert_message[1024] = "";

        GLsizei tesc_log_length = 0;
        GLchar tesc_message[1024] = "";

        GLsizei tese_log_length = 0;
        GLchar tese_message[1024] = "";

        GLsizei geom_log_length = 0;
        GLchar geom_message[1024] = "";

        GLsizei frag_log_length = 0;
        GLchar frag_message[1024] = "";

        OpenGLShader();
        ~OpenGLShader();

        void SetVertexShader(Ref<UID> vertexShader);
        void SetVertexShader(std::string vertexShader);

        void SetTesselaltionControlShader(Ref<UID> tessellationControlShader);
        void SetTesselaltionControlShader(std::string tessellationControlShader);

        void SetTesselaltionEvaluationShader(Ref<UID> tessellationEvaluationShader);
        void SetTesselaltionEvaluationShader(std::string tessellationEvaluationShader);

        void SetGeometryShader(Ref<UID> geometryShader);
        void SetGeometryShader(std::string geometryShader);

        void SetFragmentShader(Ref<UID> fragmentShader);
        void SetFragmentShader(std::string fragmentShader);

        Ref<UID> GetVertexShader();
        Ref<UID> GetFragmentShader();
        Ref<UID> GetTesselaltionControlShader();
        Ref<UID> GetTesselaltionEvaluationShader();
        Ref<UID> GetGeometryShader();

        GLuint GetID();

        virtual void Compile() override;

        virtual std::map<std::string, Ref<IShaderParameter>> GetParameters() override;

        static Ref<OpenGLShader> CreateDefaultShader();
        static Ref<OpenGLShader> CreateErrorShader();
        static Ref<OpenGLShader> CreateGridShader();
    };
}