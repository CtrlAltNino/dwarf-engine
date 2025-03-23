#pragma once

#include "Core/Rendering/Mesh/Vertex.h"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Logging/IDwarfLogger.h"
#include <glad/glad.h>

namespace Dwarf
{
  class OpenGLMesh : public IMeshBuffer
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IVramTracker> mVramTracker;
    size_t                        mVramMemory = 0;
    uint32_t                      mVertexCount = 0;
    uint32_t                      mIndexCount = 0;

  public:
    OpenGLMesh(const std::vector<Vertex>&       vertices,
               const std::vector<unsigned int>& indices,
               std::shared_ptr<IDwarfLogger>    logger,
               std::shared_ptr<IVramTracker>    vramTracker);
    ~OpenGLMesh() override;

    /**
     * @brief Binds the OpenGL mesh
     *
     */
    void
    Bind() const;

    /**
     * @brief Unbinds the OpenGL mesh
     *
     */
    void
    Unbind() const;

    /**
     * @brief Returns the vertex count of the mesh
     *
     * @return Vertex count
     */
    auto
    GetVertexCount() -> uint32_t override;

    /**
     * @brief Returns the index count of the mesh
     *
     * @return Index count
     */
    auto
    GetIndexCount() -> uint32_t override;

  private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
  };
}