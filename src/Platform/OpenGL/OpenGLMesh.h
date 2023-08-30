#pragma once

#include "Core/Rendering/Mesh.h"

namespace Dwarf {

	class OpenGLMesh : public Mesh
	{
        public:
            virtual void GenerateBuffers() override;
            void Bind();
    };
}