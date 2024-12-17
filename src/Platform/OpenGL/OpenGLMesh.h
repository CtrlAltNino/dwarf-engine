#pragma once

#include <glad/glad.h>
#include "Core/Rendering/Mesh/IMesh.h"
#include "Core/Rendering/VramTracker/IVramTracker.h"
#include "Logging/IDwarfLogger.h"

namespace Dwarf
{

  class OpenGLMesh : public IMesh
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    std::vector<Vertex>           m_Vertices = std::vector<Vertex>();
    std::vector<unsigned int>     m_Indices = std::vector<unsigned int>();
    unsigned int                  m_MaterialIndex = 0;
    std::shared_ptr<IVramTracker> m_VramTracker;
    size_t                        m_VramMemory = 0;

  public:
    OpenGLMesh(const std::vector<Vertex>&       vertices,
               const std::vector<unsigned int>& indices,
               unsigned int                     materialIndex,
               std::shared_ptr<IDwarfLogger>    logger,
               std::shared_ptr<IVramTracker>    vramTracker);
    ~OpenGLMesh() override;
    void
    SetupMesh() override;
    void
    Bind() const;
    void
    Unbind() const;

    int
    GetMaterialIndex() const override;
    std::vector<Vertex>
    GetVertices() const override;
    std::vector<unsigned int>
    GetIndices() const override;

    std::unique_ptr<IMesh>
    Clone() const override;

  private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
  };
}