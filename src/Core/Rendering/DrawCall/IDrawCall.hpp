#pragma once

#include "Core/Asset/Database/AssetComponents.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBuffer.hpp"
#include "Core/Scene/Components/TransformComponentHandle.hpp"
#include <glm/fwd.hpp>

namespace Dwarf
{
  /**
   * @brief Class that contains all data for a draw call.
   *
   */
  class IDrawCall
  {
  public:
    virtual ~IDrawCall() = default;

    /**
     * @brief Retrieves the mesh buffer of the draw call
     *
     * @return Reference to the unique pointer to the mesh buffer
     */
    virtual auto
    GetMeshBuffer() -> std::unique_ptr<IMeshBuffer>& = 0;

    /**
     * @brief Retrieves the material of the draw call
     *
     * @return Reference to the material
     */
    virtual auto
    GetMaterialAsset() -> MaterialAsset& = 0;

    /**
     * @brief Retrieves the transform of the draw call
     *
     * @return Reference to the transform component for the draw call
     */
    virtual auto
    GetTransform() -> TransformComponentHandle = 0;
  };
}