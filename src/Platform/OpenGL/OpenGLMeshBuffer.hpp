#pragma once

#include "Core/Rendering/Mesh/Vertex.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.hpp"
#include "Core/Rendering/VramTracker/IVramTracker.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <glad/glad.h>

namespace Dwarf
{
  class OpenGLMeshBuffer : public IMeshBuffer
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    std::shared_ptr<IVramTracker> mVramTracker;
    size_t                        mVramMemory = 0;
    uint32_t                      mVertexCount = 0;
    uint32_t                      mIndexCount = 0;

  public:
    OpenGLMeshBuffer(const std::vector<Vertex>&    vertices,
                     const std::vector<uint32_t>&  indices,
                     std::shared_ptr<IDwarfLogger> logger,
                     std::shared_ptr<IVramTracker> vramTracker);
    ~OpenGLMeshBuffer() override;

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
    [[nodiscard]] auto
    GetVertexCount() const -> uint32_t override;

    /**
     * @brief Returns the index count of the mesh
     *
     * @return Index count
     */
    [[nodiscard]] auto
    GetIndexCount() const -> uint32_t override;

  private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
  };
}