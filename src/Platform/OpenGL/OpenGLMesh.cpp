#include "pch.h"
#include <glad/glad.h>
#include "OpenGLMesh.h"

namespace Dwarf
{
  OpenGLMesh::OpenGLMesh(const std::vector<Vertex>&       vertices,
                         const std::vector<unsigned int>& indices,
                         unsigned int                     materialIndex)
    : m_Vertices(vertices)
    , m_Indices(indices)
    , m_MaterialIndex(materialIndex)
  {
  }
  OpenGLMesh::~OpenGLMesh()
  {
    Unbind();
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
  }

  void
  OpenGLMesh::Bind() const
  {
    glBindVertexArray(VAO);
  }

  void
  OpenGLMesh::Unbind() const
  {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  void
  OpenGLMesh::SetupMesh()
  {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 GetVertices().size() * sizeof(Vertex),
                 &(GetVertices()[0]),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 GetIndices().size() * sizeof(unsigned int),
                 &(GetIndices()[0]),
                 GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Normal));
    // vertex tangents
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Tangent));
    // vertex biTangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, BiTangent));
    // vertex texture coords
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
      4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));

    Unbind();
  }

  int
  OpenGLMesh::GetMaterialIndex() const
  {
    return m_MaterialIndex;
  }

  std::vector<Vertex>
  OpenGLMesh::GetVertices() const
  {
    return m_Vertices;
  }

  std::vector<unsigned int>
  OpenGLMesh::GetIndices() const
  {
    return m_Indices;
  }
}