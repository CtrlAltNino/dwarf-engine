#pragma once

#include "Core/Rendering/Mesh/IMesh.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.hpp"

namespace Dwarf
{
  /**
   * @brief A class that creates mesh buffer instances
   *
   */
  class IMeshBufferFactory
  {
  public:
    virtual ~IMeshBufferFactory() = default;

    /**
     * @brief Creates a mesh buffer on the gpu based on a provided mesh
     *
     * @param mesh Mesh to upload to the gpu
     * @return Unique pointer to the created mesh buffer
     */
    [[nodiscard]] virtual auto
    Create(const std::shared_ptr<IMesh>& mesh) const
      -> std::unique_ptr<IMeshBuffer> = 0;
  };
}