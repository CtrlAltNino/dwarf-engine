#pragma once

#include <glad/glad.h>
#include "Core/Rendering/Mesh.h"

namespace Dwarf
{

  class OpenGLMesh : public Mesh
  {
  public:
    using Mesh::Mesh;
    ~OpenGLMesh() override;
    void
    SetupMesh() override;
    void
    Bind() const;
    void
    Unbind() const;

  private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
  };
}