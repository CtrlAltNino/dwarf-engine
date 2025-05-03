#include "pch.hpp"

#include "OpenGLMeshBuffer.hpp"
#include "Platform/OpenGL/OpenGLUtilities.hpp"

namespace Dwarf
{
  OpenGLMeshBuffer::OpenGLMeshBuffer(const std::vector<Vertex>&    vertices,
                                     const std::vector<uint32_t>&  indices,
                                     std::shared_ptr<IDwarfLogger> logger,
                                     std::shared_ptr<IVramTracker> vramTracker)
    : mLogger(std::move(logger))
    , mVramTracker(std::move(vramTracker))
    , mVertexCount(vertices.size())
    , mIndexCount(indices.size())
  {
    mLogger->LogDebug(Log("OpenGLMeshBuffer created.", "OpenGLMeshBuffer"));
    OpenGLUtilities::CheckOpenGLError(
      "Before creating mesh", "OpenGLMeshBuffer", mLogger);
    glGenVertexArrays(1, &VAO);
    OpenGLUtilities::CheckOpenGLError(
      "glGenVertexArrays", "OpenGLMeshBuffer", mLogger);
    glGenBuffers(1, &VBO);
    OpenGLUtilities::CheckOpenGLError(
      "glGenBuffers VBO", "OpenGLMeshBuffer", mLogger);
    glGenBuffers(1, &EBO);
    OpenGLUtilities::CheckOpenGLError(
      "glGenBuffers EBO", "OpenGLMeshBuffer", mLogger);
    glBindVertexArray(VAO);
    OpenGLUtilities::CheckOpenGLError(
      "glBindVertexArray", "OpenGLMeshBuffer", mLogger);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    OpenGLUtilities::CheckOpenGLError(
      "glBindBuffer VBO", "OpenGLMeshBuffer", mLogger);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(Vertex),
                 &(vertices[0]),
                 GL_STATIC_DRAW);
    OpenGLUtilities::CheckOpenGLError(
      "glBufferData VBO", "OpenGLMeshBuffer", mLogger);

    mVramMemory += vertices.size() * sizeof(Vertex);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    OpenGLUtilities::CheckOpenGLError(
      "glBindBuffer EBO", "OpenGLMeshBuffer", mLogger);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(uint32_t),
                 &(indices[0]),
                 GL_STATIC_DRAW);
    OpenGLUtilities::CheckOpenGLError(
      "glBufferData EBO", "OpenGLMeshBuffer", mLogger);

    mVramMemory += indices.size() * sizeof(uint32_t);

    // vertex positions
    glEnableVertexAttribArray(0);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 0", "OpenGLMeshBuffer", mLogger);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 0", "OpenGLMeshBuffer", mLogger);
    // vertex normals
    glEnableVertexAttribArray(1);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 1", "OpenGLMeshBuffer", mLogger);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Normal));
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 1", "OpenGLMeshBuffer", mLogger);
    // vertex tangents
    glEnableVertexAttribArray(2);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 2", "OpenGLMeshBuffer", mLogger);
    glVertexAttribPointer(2,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Tangent));
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 2", "OpenGLMeshBuffer", mLogger);
    // vertex biTangent
    glEnableVertexAttribArray(3);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 3", "OpenGLMeshBuffer", mLogger);
    glVertexAttribPointer(3,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, BiTangent));
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 3", "OpenGLMeshBuffer", mLogger);
    // vertex texture coords
    glEnableVertexAttribArray(4);
    OpenGLUtilities::CheckOpenGLError(
      "glEnableVertexAttribArray 4", "OpenGLMeshBuffer", mLogger);
    glVertexAttribPointer(
      4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
    OpenGLUtilities::CheckOpenGLError(
      "glVertexAttribPointer 4", "OpenGLMeshBuffer", mLogger);

    mVramTracker->AddBufferMemory(mVramMemory);
    Unbind();
  }

  OpenGLMeshBuffer::~OpenGLMeshBuffer()
  {
    mLogger->LogDebug(Log("OpenGLMeshBuffer destroyed.", "OpenGLMeshBuffer"));
    Unbind();
    glDeleteVertexArrays(1, &VAO);
    OpenGLUtilities::CheckOpenGLError(
      "glDeleteVertexArrays", "OpenGLMeshBuffer", mLogger);
    glDeleteBuffers(1, &VBO);
    OpenGLUtilities::CheckOpenGLError(
      "glDeleteBuffers", "OpenGLMeshBuffer", mLogger);
    glDeleteBuffers(1, &EBO);
    OpenGLUtilities::CheckOpenGLError(
      "glDeleteBuffers", "OpenGLMeshBuffer", mLogger);
    mVramTracker->RemoveBufferMemory(mVramMemory);
  }

  void
  OpenGLMeshBuffer::Bind() const
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before binding mesh", "OpenGLMeshBuffer", mLogger);
    glBindVertexArray(VAO);
    OpenGLUtilities::CheckOpenGLError(
      "glBindVertexArray", "OpenGLMeshBuffer", mLogger);
  }

  void
  OpenGLMeshBuffer::Unbind() const
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before unbinding mesh", "OpenGLMeshBuffer", mLogger);
    glBindVertexArray(0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindVertexArray 0", "OpenGLMeshBuffer", mLogger);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindBuffer GL_ARRAY_BUFFER 0", "OpenGLMeshBuffer", mLogger);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindBuffer GL_ELEMENT_ARRAY_BUFFER 0", "OpenGLMeshBuffer", mLogger);
  }

  auto
  OpenGLMeshBuffer::GetVertexCount() const -> uint32_t
  {
    return mVertexCount;
  }

  auto
  OpenGLMeshBuffer::GetIndexCount() const -> uint32_t
  {
    return mIndexCount;
  }
}