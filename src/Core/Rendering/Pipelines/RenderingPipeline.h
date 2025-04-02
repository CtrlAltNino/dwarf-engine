#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallList.h"
#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallListFactory.h"
#include "Core/Rendering/DrawCall/DrawCallWorker/IDrawCallWorker.h"
#include "Core/Rendering/DrawCall/DrawCallWorker/IDrawCallWorkerFactory.h"
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
    std::shared_ptr<IMaterial>    mTonemapMaterial;

    std::shared_ptr<IRendererApi>    mRendererApi;
    std::unique_ptr<IDrawCallList>   mDrawCallList;
    std::unique_ptr<IDrawCallWorker> mDrawCallWorker;

  public:
    RenderingPipeline(
      std::shared_ptr<IRendererApi>     rendererApi,
      std::shared_ptr<IMaterialFactory> materialFactory,
      std::shared_ptr<IShaderRegistry>  shaderRegistry,
      std::shared_ptr<IShaderSourceCollectionFactory>
                                           shaderSourceCollectionFactory,
      std::shared_ptr<IMeshFactory>        meshFactory,
      std::shared_ptr<IMeshBufferFactory>  meshBufferFactory,
      std::shared_ptr<IFramebufferFactory> framebufferFactory,
      const std::shared_ptr<IDrawCallListFactory>&   drawCallListFactory,
      const std::shared_ptr<IDrawCallWorkerFactory>& drawCallWorkerFactory);
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

    /**
     * @brief Gets the resolution
     *
     * @return glm::ivec2 resolution
     */
    auto
    GetResolution() -> glm::ivec2 override;

    /**
     * @brief Sets the resolution
     *
     * @param resolution resolution in pixels
     */
    void
    SetResolution(glm::ivec2 resolution) override;

    /**
     * @brief Sets the MSAA sample amount
     *
     * @param samples Amount of samples per pixels
     */
    void
    SetMsaaSamples(int32_t samples) override;

    /**
     * @brief Gets the MSAA sample amount
     *
     * @return Amount of samples
     */
    auto
    GetMsaaSamples() -> int32_t override;

    /**
     * @brief Gets the maximum amount of MSAA samples supported on the device
     *
     * @return Maximum amount of samples
     */
    auto
    GetMaxMsaaSamples() -> int32_t override;

    /**
     * @brief Reads the id from the id framebuffer at a given position
     *
     * @param position 2d pixel position
     * @return Stored ID
     */
    auto
    ReadPixelId(glm::ivec2 position) -> uint32_t override;

    /**
     * @brief Gets the Texture ID for the presentation buffer
     *
     * @return Id to the buffer
     */
    auto
    GetPresentationBufferId() -> uintptr_t override;

    /**
     * @brief Sets the exposure value to apply before tone mapping
     *
     * @param exposure Exposure value
     */
    void
    SetExposure(float exposure) override;
  };
}