#pragma once

#include "Core/Rendering/Framebuffer/IFramebufferFactory.hpp"
#include "Core/Rendering/Mesh/IMeshFactory.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.hpp"
#include "Core/Scene/Camera/ICameraFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class MaterialPreview : public IMaterialPreview
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    MaterialPreviewMeshType       mMeshType = MaterialPreviewMeshType::Sphere;
    std::shared_ptr<IMeshBufferFactory> mMeshBufferFactory;
    std::shared_ptr<IMeshFactory>       mMeshFactory;
    std::unique_ptr<IMeshBuffer>        mMeshBuffer;

    void
    UpdateMesh();

  public:
    MaterialPreview(
      std::shared_ptr<IDwarfLogger>               logger,
      std::shared_ptr<IFramebufferFactory>        framebufferFactory,
      const std::shared_ptr<ICameraFactory>&      cameraFactory,
      const std::shared_ptr<IRendererApiFactory>& rendererApiFactory,
      std::shared_ptr<IMeshBufferFactory>         meshBufferFactory,
      std::shared_ptr<IMeshFactory>               meshFactory,
      std::shared_ptr<IEditorStats>               editorStats);
    ~MaterialPreview() override;

    /**
     * @brief Renders a material to preview it
     *
     * @param material Material to preview
     */
    void
    RenderMaterialPreview(IMaterial& material) override;

    /**
     * @brief Sets the desired mesh type to preview a material on
     *
     * @param meshType Mesh type
     */
    void
    SetMeshType(MaterialPreviewMeshType meshType) override;

    /**
     * @brief Retrieves the currently used mesh type
     *
     * @return Current mesh type
     */
    [[nodiscard]] auto
    GetMeshType() const -> MaterialPreviewMeshType override;
  };
}