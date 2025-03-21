#pragma once

#include "Core/Rendering/PreviewRenderer/PreviewRenderer.h"
#include <imgui.h>
namespace Dwarf
{
  /**
   * @brief Enum representing the different mesh options to apply the material
   * to
   *
   */
  enum class MaterialPreviewMeshType
  {
    Sphere,
    Cube,
    Plane
  };

  /**
   * @brief A class that manages the preview of a material
   *
   */
  class IMaterialPreview : public virtual PreviewRenderer
  {
  public:
    virtual ~IMaterialPreview() = default;

    /**
     * @brief Renders a material to preview it
     *
     * @param material Material to preview
     */
    virtual void
    RenderMaterialPreview(IMaterial& material) = 0;

    /**
     * @brief Sets the desired mesh type to preview a material on
     *
     * @param meshType Mesh type
     */
    virtual void
    SetMeshType(MaterialPreviewMeshType meshType) = 0;

    /**
     * @brief Retrieves the currently used mesh type
     *
     * @return Current mesh type
     */
    [[nodiscard]] virtual auto
    GetMeshType() const -> MaterialPreviewMeshType = 0;
  };
}