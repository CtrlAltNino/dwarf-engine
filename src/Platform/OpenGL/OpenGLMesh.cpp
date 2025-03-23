#include "OpenGLMesh.h"
#include "Platform/OpenGL/OpenGLUtilities.h"
#include "pch.h"


namespace Dwarf
{
  OpenGLMesh::OpenGLMesh(const std::vector<Vertex>&       vertices,
                         const std::vector<unsigned int>& indices,
                         std::shared_ptr<IDwarfLogger>    logger,
                         std::shared_ptr<IVramTracker>    vramTracker)
    : mLogger(std::move(logger))
    , mVramTracker(std::move(vramTracker))
    , mVertexCount(vertices.size())
    , mIndexCount(indices.size())
  {
    mLogger->LogDebug(Log("OpenGLMesh created.", "OpenGLMesh"));
    OpenGLUtilities::CheckOpenGLError(
      "Before creating mesh", "OpenGLMesh", mLogger);
    glGenVertexArrays(1, &VAO);
    OpenGLUtilities::CheckOpenGLError(
      "glGenVertexArrays", "OpenGLMesh", mLogger);
    glGenBuffers(1, &VBO);
    OpenGLUtilities::CheckOpenGLError(
      "glGenBuffers VBO", "OpenGLMesh", mLogger);
    glGenBuffers(1, &EBO);
    OpenGLUtilities::CheckOpenGLError(
      "glGenBuffers EBO", "OpenGLMesh", mLogger);
    glBindVertexArray(VAO);
    OpenGLUtilities::CheckOpenGLError(
      "glBindVertexArray", "OpenGLMesh", mLogger);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    OpenGLUtilities::CheckOpenGLError(
      "glBindBuffer VBO", "OpenGLMesh", mLogger);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(Vertex),
                 &(vertices[0]),
                 GL_STATIC_DRAW);
    OpenGLUtilities::CheckOpenGLError(
      "glBufferData VBO", "OpenGLMesh", mLogger);

    mVramMemory += vertices.size() * sizeof(Vertex);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    OpenGLUtilities::CheckOpenGLError(
      "glBindBuffer EBO", "OpenGLMesh", mLogger);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 &(indices[0]),
                 GL_STATIC_DRAW);
    OpenGLUtilities::CheckOpenGLError(
      "glBufferData EBO", "OpenGLMesh", mLogger);

    mVramMemory += indices.size() * sizeof(unsigned int);

    // vertex positions
    glEnableVertexAttribArray(0);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 0", "OpenGLMesh", mLogger);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 0", "OpenGLMesh", mLogger);
    // vertex normals
    glEnableVertexAttribArray(1);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 1", "OpenGLMesh", mLogger);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Normal));
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 1", "OpenGLMesh", mLogger);
    // vertex tangents
    glEnableVertexAttribArray(2);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 2", "OpenGLMesh", mLogger);
    glVertexAttribPointer(2,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Tangent));
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 2", "OpenGLMesh", mLogger);
    // vertex biTangent
    glEnableVertexAttribArray(3);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 3", "OpenGLMesh", mLogger);
    glVertexAttribPointer(3,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, BiTangent));
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 3", "OpenGLMesh", mLogger);
    // vertex texture coords
    glEnableVertexAttribArray(4);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 4", "OpenGLMesh", mLogger);
    glVertexAttribPointer(
      4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 4", "OpenGLMesh", mLogger);

    mVramTracker->AddBufferMemory(mVramMemory);
    Unbind();
  }

  OpenGLMesh::~OpenGLMesh()
  {
    mLogger->LogDebug(Log("OpenGLMesh destroyed.", "OpenGLMesh"));
    Unbind();
    glDeleteVertexArrays(1, &VAO);
    OpenGLUtilities::CheckOpenGLError(
      "glDeleteVertexArrays", "OpenGLMesh", mLogger);
    glDeleteBuffers(1, &VBO);
    OpenGLUtilities::CheckOpenGLError("glDeleteBuffers", "OpenGLMesh", mLogger);
    glDeleteBuffers(1, &EBO);
    OpenGLUtilities::CheckOpenGLError("glDeleteBuffers", "OpenGLMesh", mLogger);
    mVramTracker->RemoveBufferMemory(mVramMemory);
  }

  void
  OpenGLMesh::Bind() const
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before binding mesh", "OpenGLMesh", mLogger);
    glBindVertexArray(VAO);
    OpenGLUtilities::CheckOpenGLError(
      "glBindVertexArray", "OpenGLMesh", mLogger);
  }

  void
  OpenGLMesh::Unbind() const
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before unbinding mesh", "OpenGLMesh", mLogger);
    glBindVertexArray(0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindVertexArray 0", "OpenGLMesh", mLogger);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindBuffer GL_ARRAY_BUFFER 0", "OpenGLMesh", mLogger);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindBuffer GL_ELEMENT_ARRAY_BUFFER 0", "OpenGLMesh", mLogger);
  }

  auto
  OpenGLMesh::GetVertexCount() -> uint32_t
  {
    return mVertexCount;
  }

  auto
  OpenGLMesh::GetIndexCount() -> uint32_t
  {
    return mIndexCount;
  }
}