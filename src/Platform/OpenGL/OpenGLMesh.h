#pragma once

#include <glad/glad.h>
#include "Core/Rendering/Mesh/Vertex.h"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{
  class OpenGLMesh : public IMeshBuffer
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    std::shared_ptr<IVramTracker> m_VramTracker;
    size_t                        m_VramMemory = 0;
    uint32_t                      m_VertexCount = 0;
    uint32_t                      m_IndexCount = 0;

  public:
    OpenGLMesh(const std::vector<Vertex>&       vertices,
               const std::vector<unsigned int>& indices,
               std::shared_ptr<IDwarfLogger>    logger,
               std::shared_ptr<IVramTracker>    vramTracker);
    ~OpenGLMesh() override;
    void
    Bind() const;
    void
    Unbind() const;

    uint32_t
    GetVertexCount() override;

    uint32_t
    GetIndexCount() override;

  private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
  };
}