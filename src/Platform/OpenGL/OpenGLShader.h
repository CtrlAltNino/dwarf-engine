#pragma once

#include "Core/Rendering/Shader.h"
#include "Core/Base.h"

namespace Dwarf {
    class OpenGLShader : public Shader{
        private:
            GLuint m_ID = -1;

            Ref<UID> m_VertexShader;
            Ref<UID> m_FragmentShader;
            Ref<UID> m_GeometryShader;
        public:
            OpenGLShader();
            ~OpenGLShader();
            
            void SetVertexShader(Ref<UID> vertexShader);
            void SetFragmentShader(Ref<UID> fragmentShader);
            void SetGeometryShader(Ref<UID> geometryShader);

            Ref<UID> GetVertexShader();
            Ref<UID> GetFragmentShader();
            Ref<UID> GetGeometryShader();
            
            GLuint GetID();
            
            virtual void Compile() override;
            
            static Ref<OpenGLShader> CreateDefaultShader();
            static Ref<OpenGLShader> CreateErrorShader();
            static Ref<OpenGLShader> CreateGridShader();
    };
}