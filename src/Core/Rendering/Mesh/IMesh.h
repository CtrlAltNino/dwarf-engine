#pragma once

#include "Core/Rendering/Mesh/Vertex.h"

namespace Dwarf
{
  /**
   * @brief Class that represents a mesh
   *
   */
  class IMesh
  {
  public:
    virtual ~IMesh() = default;

    /**
     * @brief Retrieves the material index of the mesh
     *
     * @return Material index
     */
    [[nodiscard]] virtual auto
    GetMaterialIndex() const -> uint32_t = 0;

    /**
     * @brief Returns a reference to the stored vertex list
     *
     * @return Immutable reference to the stored vertex vector
     */
    [[nodiscard]] virtual auto
    GetVertices() const -> const std::vector<Vertex>& = 0;

    /**
     * @brief Returns a reference to the stored index list
     *
     * @return Immutable reference to the stored index vector
     */
    [[nodiscard]] virtual auto
    GetIndices() const -> const std::vector<uint32_t>& = 0;

    /**
     * @brief Clones the Mesh instance
     *
     * @return Unique pointer to the cloned Mesh instance
     */
    [[nodiscard]] virtual auto
    Clone() const -> std::unique_ptr<IMesh> = 0;
  };
}