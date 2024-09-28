#include "Platform/OpenGL/OpenGLUtilities.h"
#include "pch.h"
#include "OpenGLMesh.h"

namespace Dwarf
{
  OpenGLMesh::OpenGLMesh(const std::vector<Vertex>&       vertices,
                         const std::vector<unsigned int>& indices,
                         unsigned int                     materialIndex,
                         std::shared_ptr<IDwarfLogger>    logger)
    : m_Vertices(vertices)
    , m_Indices(indices)
    , m_MaterialIndex(materialIndex)
    , m_Logger(logger)
  {
    m_Logger->LogInfo(Log("OpenGLMesh created.", "OpenGLMesh"));
  }

  OpenGLMesh::~OpenGLMesh()
  {
    m_Logger->LogInfo(Log("OpenGLMesh destroyed.", "OpenGLMesh"));
    Unbind();
    glDeleteVertexArrays(1, &VAO);
    OpenGLUtilities::CheckOpenGLError(
      "glDeleteVertexArrays", "OpenGLMesh", m_Logger);
    glDeleteBuffers(1, &VBO);
    OpenGLUtilities::CheckOpenGLError(
      "glDeleteBuffers", "OpenGLMesh", m_Logger);
    glDeleteBuffers(1, &EBO);
    OpenGLUtilities::CheckOpenGLError(
      "glDeleteBuffers", "OpenGLMesh", m_Logger);
  }

  void
  OpenGLMesh::Bind() const
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before binding mesh", "OpenGLMesh", m_Logger);
    glBindVertexArray(VAO);
    OpenGLUtilities::CheckOpenGLError(
      "glBindVertexArray", "OpenGLMesh", m_Logger);
  }

  void
  OpenGLMesh::Unbind() const
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before unbinding mesh", "OpenGLMesh", m_Logger);
    glBindVertexArray(0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindVertexArray 0", "OpenGLMesh", m_Logger);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindBuffer GL_ARRAY_BUFFER 0", "OpenGLMesh", m_Logger);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindBuffer GL_ELEMENT_ARRAY_BUFFER 0", "OpenGLMesh", m_Logger);
  }

  void
  OpenGLMesh::SetupMesh()
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before creating mesh", "OpenGLMesh", m_Logger);
    glGenVertexArrays(1, &VAO);
    OpenGLUtilities::CheckOpenGLError(
      "glGenVertexArrays", "OpenGLMesh", m_Logger);
    glGenBuffers(1, &VBO);
    OpenGLUtilities::CheckOpenGLError(
      "glGenBuffers VBO", "OpenGLMesh", m_Logger);
    glGenBuffers(1, &EBO);
    OpenGLUtilities::CheckOpenGLError(
      "glGenBuffers EBO", "OpenGLMesh", m_Logger);
    glBindVertexArray(VAO);
    OpenGLUtilities::CheckOpenGLError(
      "glBindVertexArray", "OpenGLMesh", m_Logger);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    OpenGLUtilities::CheckOpenGLError(
      "glBindBuffer VBO", "OpenGLMesh", m_Logger);
    glBufferData(GL_ARRAY_BUFFER,
                 GetVertices().size() * sizeof(Vertex),
                 &(GetVertices()[0]),
                 GL_STATIC_DRAW);
    OpenGLUtilities::CheckOpenGLError(
      "glBufferData VBO", "OpenGLMesh", m_Logger);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    OpenGLUtilities::CheckOpenGLError(
      "glBindBuffer EBO", "OpenGLMesh", m_Logger);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 GetIndices().size() * sizeof(unsigned int),
                 &(GetIndices()[0]),
                 GL_STATIC_DRAW);
    OpenGLUtilities::CheckOpenGLError(
      "glBufferData EBO", "OpenGLMesh", m_Logger);

    // vertex positions
    glEnableVertexAttribArray(0);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 0", "OpenGLMesh", m_Logger);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 0", "OpenGLMesh", m_Logger);
    // vertex normals
    glEnableVertexAttribArray(1);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 1", "OpenGLMesh", m_Logger);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Normal));
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 1", "OpenGLMesh", m_Logger);
    // vertex tangents
    glEnableVertexAttribArray(2);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 2", "OpenGLMesh", m_Logger);
    glVertexAttribPointer(2,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Tangent));
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 2", "OpenGLMesh", m_Logger);
    // vertex biTangent
    glEnableVertexAttribArray(3);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 3", "OpenGLMesh", m_Logger);
    glVertexAttribPointer(3,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, BiTangent));
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 3", "OpenGLMesh", m_Logger);
    // vertex texture coords
    glEnableVertexAttribArray(4);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 4", "OpenGLMesh", m_Logger);
    glVertexAttribPointer(
      4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 4", "OpenGLMesh", m_Logger);

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

  std::unique_ptr<IMesh>
  OpenGLMesh::Clone() const
  {
    return std::make_unique<OpenGLMesh>(
      m_Vertices, m_Indices, m_MaterialIndex, m_Logger);
  }
}