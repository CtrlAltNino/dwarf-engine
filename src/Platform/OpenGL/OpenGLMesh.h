#pragma once

#include "Core/Rendering/Mesh.h"

#include <glad/glad.h>

namespace Dwarf
{

    class OpenGLMesh : public Mesh
    {
    public:
        OpenGLMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, unsigned int materialIndex);
        ~OpenGLMesh();
        void SetupMesh() override;
        void Bind();
        void Unbind();

    private:
        GLuint VAO, VBO, EBO;
    };
}