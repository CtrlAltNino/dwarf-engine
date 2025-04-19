#pragma once

namespace Dwarf
{
  /**
   * @brief A class representing a mesh on the GPU
   *
   */
  class IMeshBuffer
  {
  public:
    IMeshBuffer() = default;
    virtual ~IMeshBuffer() = default;

    /**
     * @brief Returns the vertex count of the mesh
     *
     * @return Vertex count
     */
    virtual auto
    GetVertexCount() -> uint32_t = 0;

    /**
     * @brief Returns the index count of the mesh
     *
     * @return Index count
     */
    virtual auto
    GetIndexCount() -> uint32_t = 0;

    IMeshBuffer(const IMeshBuffer&) = delete; // Delete copy constructor
    auto
    operator=(const IMeshBuffer&)
      -> IMeshBuffer& = delete; // Delete copy assignment operator
  };
}