#pragma once

#include "Core/Rendering/Mesh.h"

#include <glad/glad.h>

namespace Dwarf
{

    class OpenGLMesh : public Mesh
    {
    public:
        using Mesh::Mesh;
        ~OpenGLMesh() override;
        void SetupMesh() override;
        void Bind() const;
        void Unbind() const;

    private:
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
    };
}