#pragma once

#include "Core/Scene/Components/SceneComponents.h"
#include "IDrawCall.h"

namespace Dwarf
{
  class DrawCall : public IDrawCall
  {
  private:
    std::unique_ptr<IMeshBuffer> mMeshBuffer;
    IMaterial&                   mMaterial;
    TransformComponent&          mTransform;

  public:
    DrawCall(std::unique_ptr<IMeshBuffer>&& meshBuffer,
             IMaterial&                     material,
             TransformComponent&            transform);

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
    GetMaterial() -> IMaterial& override;

    /**
     * @brief Retrieves the transform of the draw call
     *
     * @return Reference to the transform component for the draw call
     */
    auto
    GetTransform() -> TransformComponent& override;
  };
}