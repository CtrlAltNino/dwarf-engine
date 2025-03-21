#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallList.h"
#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "Core/Rendering/Pipelines/IRenderingPipeline.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.h"
#include <memory>

namespace Dwarf
{
  class RenderingPipeline : public IRenderingPipeline
  {
  private:
    std::unique_ptr<IMaterial>        mIdMaterial;
    std::unique_ptr<IMaterial>        mGridMaterial;
    std::unique_ptr<IMeshBuffer>      mGridMeshBuffer;
    glm::mat4                         mGridModelMatrix{};
    std::shared_ptr<IRendererApi>     mRendererApi;
    std::shared_ptr<IMaterialFactory> mMaterialFactory;
    std::shared_ptr<IShaderRegistry>  mShaderRegistry;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        mShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>       mMeshFactory;
    std::shared_ptr<IMeshBufferFactory> mMeshBufferFactory;
    std::shared_ptr<IDrawCallList>      mDrawCallList;

    void
    Setup(glm::ivec2 viewportSize);

  public:
    RenderingPipeline(std::shared_ptr<IRendererApi>     rendererApi,
                      std::shared_ptr<IMaterialFactory> materialFactory,
                      std::shared_ptr<IShaderRegistry>  shaderRegistry,
                      std::shared_ptr<IShaderSourceCollectionFactory>
                        shaderSourceCollectionFactory,
                      std::shared_ptr<IMeshFactory>       meshFactory,
                      std::shared_ptr<IMeshBufferFactory> meshBufferFactory,
                      std::shared_ptr<IDrawCallList>      drawCallList);
    ~RenderingPipeline() override;

    /**
     * @brief Renders the current draw calls of the scene
     *
     * @param camera Camera to use for rendering
     * @param viewportSize Viewport to render
     * @param renderGrid Should the grid be rendered
     */
    void
    RenderScene(ICamera&   camera,
                glm::ivec2 viewportSize,
                bool       renderGrid) override;

    /**
     * @brief Returns the specification for the framebuffer
     *
     * @return Framebuffer specification
     */
    [[nodiscard]] auto
    GetSpecification() const -> FramebufferSpecification override;

    /**
     * @brief Renders the scene to the ID buffer for mouse picking
     *
     * @param scene Scene to render
     * @param camera Camera to use for rendering
     * @param viewportSize Viewport to render
     */
    void
    RenderIds(IScene& scene, ICamera& camera, glm::ivec2 viewportSize) override;
  };
}