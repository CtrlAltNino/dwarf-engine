#pragma once

#include "Core/Rendering/Shader.h"
#include "Core/Base.h"

namespace Dwarf{
    class MetalShader: public Shader {
        private:
            //GLuint m_ID = -1;

            Ref<UID> m_ShaderAsset;
        public:
            MetalShader();
            ~MetalShader();
            
            void SetShader(Ref<UID> shaderAsset);

            Ref<UID> GetShader();
            
            //GLuint GetID();
            
            virtual void Compile() override;
            
            static Ref<MetalShader> CreateDefaultShader();
            static Ref<MetalShader> CreateErrorShader();
            static Ref<MetalShader> CreateGridShader();
    };
}