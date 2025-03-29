#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallList.h"
#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
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
    std::shared_ptr<IMaterialFactory>    mMaterialFactory;
    std::shared_ptr<IShaderRegistry>     mShaderRegistry;
    std::shared_ptr<IFramebufferFactory> mFramebufferFactory;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        mShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>       mMeshFactory;
    std::shared_ptr<IMeshBufferFactory> mMeshBufferFactory;

    std::unique_ptr<IMaterial>    mIdMaterial;
    std::unique_ptr<IMaterial>    mGridMaterial;
    std::unique_ptr<IMeshBuffer>  mGridMeshBuffer;
    glm::mat4                     mGridModelMatrix{};
    std::shared_ptr<IFramebuffer> mFramebuffer;
    std::shared_ptr<IFramebuffer> mNonMsaaBuffer;
    std::shared_ptr<IFramebuffer> mIdBuffer;
    std::shared_ptr<IFramebuffer> mOutlineBuffer;
    std::shared_ptr<IFramebuffer> mPresentationBuffer;
    std::shared_ptr<IShader>      mAgxTonemapShader;

    std::shared_ptr<IRendererApi>  mRendererApi;
    std::shared_ptr<IDrawCallList> mDrawCallList;

  public:
    RenderingPipeline(std::shared_ptr<IRendererApi>     rendererApi,
                      std::shared_ptr<IMaterialFactory> materialFactory,
                      std::shared_ptr<IShaderRegistry>  shaderRegistry,
                      std::shared_ptr<IShaderSourceCollectionFactory>
                        shaderSourceCollectionFactory,
                      std::shared_ptr<IMeshFactory>        meshFactory,
                      std::shared_ptr<IMeshBufferFactory>  meshBufferFactory,
                      std::shared_ptr<IDrawCallList>       drawCallList,
                      std::shared_ptr<IFramebufferFactory> framebufferFactory);
    ~RenderingPipeline() override;

    /**
     * @brief Renders the current draw calls of the scene
     *
     * @param camera Camera to use for rendering
     * @param viewportSize Viewport to render
     * @param renderGrid Should the grid be rendered
     */
    void
    RenderScene(ICamera& camera, bool renderGrid) override;

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
    RenderIds(IScene& scene, ICamera& camera) override;

    auto
    GetResolution() -> glm::ivec2 override;

    void
    SetResolution(glm::ivec2 resolution) override;

    void
    SetMsaaSamples(int32_t samples) override;

    auto
    GetMsaaSamples() -> int32_t override;

    auto
    GetMaxMsaaSamples() -> int32_t override;

    auto
    ReadPixelId(glm::ivec2 position) -> uint32_t override;

    auto
    GetPresentationBufferId() -> uintptr_t override;
  };
}