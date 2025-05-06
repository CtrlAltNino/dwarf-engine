#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.hpp"
#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallList.hpp"
#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallListFactory.hpp"
#include "Core/Rendering/DrawCall/DrawCallWorker/IDrawCallWorker.hpp"
#include "Core/Rendering/DrawCall/DrawCallWorker/IDrawCallWorkerFactory.hpp"
#include "Core/Rendering/Framebuffer/IFramebuffer.hpp"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.hpp"
#include "Core/Rendering/Material/IMaterialFactory.hpp"
#include "Core/Rendering/Mesh/IMeshFactory.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "Core/Rendering/PingPongBuffer/IPingPongBufferFactory.hpp"
#include "Core/Rendering/Pipelines/IRenderingPipeline.hpp"
#include "Core/Rendering/RendererApi/IRendererApi.hpp"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.hpp"
#include "Core/Rendering/SkyboxRenderer/ISkyboxRenderer.hpp"
#include "Core/Scene/Settings/ISceneSettingsObserver.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "IRenderingPipeline.hpp"
#include <memory>

namespace Dwarf
{
  class RenderingPipeline
    : public IRenderingPipeline
    , public ISceneSettingsObserver
    , public ILoadedSceneObserver
  {
  private:
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        mShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>       mMeshFactory;
    std::shared_ptr<IMeshBufferFactory> mMeshBufferFactory;
    std::shared_ptr<IShaderRegistry>    mShaderRegistry;
    std::shared_ptr<ILoadedScene>       mLoadedScene;
    std::shared_ptr<ISkyboxRenderer>    mSkyboxRenderer;

    std::unique_ptr<IMaterial> mIdMaterial;
    std::shared_ptr<IShader>   mGridShader;
    std::shared_ptr<IShader>   mTonemapShader;

    std::shared_ptr<IFramebuffer>    mRenderFramebuffer;
    std::unique_ptr<IPingPongBuffer> mHdrPingPong;
    std::unique_ptr<IPingPongBuffer> mLdrPingPong;
    std::shared_ptr<IFramebuffer>    mPresentationBuffer;
    std::shared_ptr<IFramebuffer>    mIdBuffer;

    std::shared_ptr<IRendererApi>    mRendererApi;
    std::unique_ptr<IDrawCallList>   mDrawCallList;
    std::unique_ptr<IDrawCallWorker> mDrawCallWorker;

    void
    SetupRenderFramebuffer(
      const std::shared_ptr<IFramebufferFactory>& framebufferFactory);

    void
    SetupNonMsaaFramebuffer(
      const std::shared_ptr<IFramebufferFactory>& framebufferFactory);

    void
    SetupPingPongBuffers(
      const std::shared_ptr<IPingPongBufferFactory>& pingPongBufferFactory);

    void
    SetupPresentationFramebuffer(
      const std::shared_ptr<IFramebufferFactory>& framebufferFactory);

    void
    SetupIdFramebuffer(
      const std::shared_ptr<IFramebufferFactory>& framebufferFactory);

  public:
    RenderingPipeline(
      std::shared_ptr<IRendererApi>    rendererApi,
      std::shared_ptr<IShaderRegistry> shaderRegistry,
      std::shared_ptr<IShaderSourceCollectionFactory>
                                                  shaderSourceCollectionFactory,
      std::shared_ptr<IMeshFactory>               meshFactory,
      std::shared_ptr<IMeshBufferFactory>         meshBufferFactory,
      std::shared_ptr<ILoadedScene>               loadedScene,
      std::shared_ptr<ISkyboxRenderer>            skyboxRenderer,
      const std::shared_ptr<IFramebufferFactory>& framebufferFactory,
      const std::shared_ptr<IMaterialFactory>&    materialFactory,
      const std::shared_ptr<IDrawCallListFactory>&   drawCallListFactory,
      const std::shared_ptr<IDrawCallWorkerFactory>& drawCallWorkerFactory,
      const std::shared_ptr<IPingPongBufferFactory>& pingPongBufferFactory);
    ~RenderingPipeline() override;

    /**
     * @brief Renders the current draw calls of the scene
     *
     * @param camera Camera to use for rendering
     * @param viewportSize Viewport to render
     * @param renderGrid Should the grid be rendered
     */
    void
    RenderScene(ICamera& camera, GridSettingsData gridSettings) override;

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

    /**
     * @brief Gets the amount of current draw calls
     *
     * @return Draw call count
     */
    [[nodiscard]] auto
    GetDrawCallCount() const -> uint32_t override;

    /**
     * @brief Gets the total amount of vertices over all draw calls
     *
     * @return Vertex count
     */
    [[nodiscard]] auto
    GetVertexCount() const -> uint32_t override;

    /**
     * @brief Gets the total amount of triangles over all draw calls
     *
     * @return Triangle count
     */
    [[nodiscard]] auto
    GetTriangleCount() const -> uint32_t override;

    /**
     * @brief Sets the used tonemap
     *
     * @param type Reinhard, Agx or Aces
     */
    void
    SetTonemapType(TonemapType type) override;

    void
    OnAntiAliasingSettingsChanged() override;

    void
    OnExposureSettingsChanged() override;

    void
    OnTonemapChanged() override;

    void
    OnBloomSettingsChanged() override;

    void
    OnSkyboxSettingsChanged() override
    {
    }

    void
    OnSceneLoad() override;

    void
    OnSceneUnload() override;
  };
}