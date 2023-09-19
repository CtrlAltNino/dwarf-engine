#pragma once

#include "Core/Rendering/Shader.h"
#include "Core/Base.h"

#include <glad/glad.h>

namespace Dwarf {
    class OpenGLShader : public Shader{
        private:
            GLuint m_ID = -1;

        public:
            Ref<UID> m_VertexShader;
            Ref<UID> m_TessellationControlShader;
            Ref<UID> m_TessellationEvaluationShader;
            Ref<UID> m_GeometryShader;
            Ref<UID> m_FragmentShader;

            GLsizei vert_log_length;
            GLchar vert_message[1024] = "";

            GLsizei tesc_log_length;
            GLchar tesc_message[1024] = "";

            GLsizei tese_log_length;
            GLchar tese_message[1024] = "";

            GLsizei geom_log_length;
            GLchar geom_message[1024] = "";

            GLsizei frag_log_length;
            GLchar frag_message[1024] = "";

            OpenGLShader();
            ~OpenGLShader();

            void SetVertexShader(Ref<UID> vertexShader);
            void SetTesselaltionControlShader(Ref<UID> tessellationControlShader);
            void SetTesselaltionEvaluationShader(Ref<UID> tessellationEvaluationShader);
            void SetFragmentShader(Ref<UID> fragmentShader);
            void SetGeometryShader(Ref<UID> geometryShader);

            Ref<UID> GetVertexShader();
            Ref<UID> GetFragmentShader();
            Ref<UID> GetTesselaltionControlShader();
            Ref<UID> GetTesselaltionEvaluationShader();
            Ref<UID> GetGeometryShader();

            GLuint GetID();

            virtual void Compile() override;

            static Ref<OpenGLShader> CreateDefaultShader();
            static Ref<OpenGLShader> CreateErrorShader();
            static Ref<OpenGLShader> CreateGridShader();
    };
}