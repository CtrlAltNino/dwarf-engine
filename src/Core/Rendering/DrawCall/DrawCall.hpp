#pragma once

#include "Core/Asset/Database/AssetComponents.hpp"
#include "IDrawCall.hpp"

namespace Dwarf
{
  class DrawCall : public IDrawCall
  {
  private:
    std::unique_ptr<IMeshBuffer> mMeshBuffer;
    MaterialAsset&               mMaterial;
    TransformComponentHandle     mTransform;

  public:
    DrawCall(std::unique_ptr<IMeshBuffer>&& meshBuffer,
             MaterialAsset&                 material,
             TransformComponentHandle       transform);

    ~DrawCall() override = default;

    /**
     * @brief Retrieves the mesh buffer of the draw call
     *
     * @return Reference to the unique pointer to the mesh buffer
     */
    auto
    GetMeshBuffer() -> std::unique_ptr<IMeshBuffer>& override;

    /**
     * @brief Retrieves the material of the draw call
     *
     * @return Reference to the material
     */
    auto
    GetMaterialAsset() -> MaterialAsset& override;

    /**
     * @brief Retrieves the transform of the draw call
     *
     * @return Reference to the transform component for the draw call
     */
    auto
    GetTransform() -> TransformComponentHandle override;
  };
}