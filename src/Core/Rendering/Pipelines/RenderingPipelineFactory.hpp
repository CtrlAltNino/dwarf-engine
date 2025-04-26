#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.hpp"
#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallListFactory.hpp"
#include "Core/Rendering/DrawCall/DrawCallWorker/IDrawCallWorkerFactory.hpp"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.hpp"
#include "Core/Rendering/Material/IMaterialFactory.hpp"
#include "Core/Rendering/Mesh/IMeshFactory.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "Core/Rendering/PingPongBuffer/IPingPongBufferFactory.hpp"
#include "Core/Rendering/RendererApi/IRendererApi.hpp"
#include "Core/Rendering/RendererApi/IRendererApiFactory.hpp"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.hpp"
#include "Editor/LoadedScene/ILoadedScene.hpp"
#include "IRenderingPipelineFactory.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class RenderingPipelineFactory : public IRenderingPipelineFactory
  {
  private:
    std::shared_ptr<IDwarfLogger>     mLogger;
    std::shared_ptr<IRendererApi>     mRendererApi;
    std::shared_ptr<IMaterialFactory> mMaterialFactory;
    std::shared_ptr<IShaderRegistry>  mShaderRegistry;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                            mShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>           mMeshFactory;
    std::shared_ptr<IMeshBufferFactory>     mMeshBufferFactory;
    std::shared_ptr<IFramebufferFactory>    mFramebufferFactory;
    std::shared_ptr<IDrawCallListFactory>   mDrawCallListFactory;
    std::shared_ptr<IDrawCallWorkerFactory> mDrawCallWorkerFactory;
    std::shared_ptr<IPingPongBufferFactory> mPingPongBufferFactory;
    std::shared_ptr<ILoadedScene>           mLoadedScene;

  public:
    RenderingPipelineFactory(
      std::shared_ptr<IDwarfLogger>               logger,
      const std::shared_ptr<IRendererApiFactory>& rendererApiFactory,
      std::shared_ptr<IMaterialFactory>           materialFactory,
      std::shared_ptr<IShaderRegistry>            shaderRegistry,
      std::shared_ptr<IShaderSourceCollectionFactory>
                                              shaderSourceCollectionFactory,
      std::shared_ptr<IMeshFactory>           meshFactory,
      std::shared_ptr<IMeshBufferFactory>     meshBufferFactory,
      std::shared_ptr<IFramebufferFactory>    framebufferFactory,
      std::shared_ptr<IDrawCallListFactory>   drawCallListFactory,
      std::shared_ptr<IDrawCallWorkerFactory> drawCallWorkerFactory,
      std::shared_ptr<IPingPongBufferFactory> pingPongBufferFactory,
      std::shared_ptr<ILoadedScene>           loadedScene);

    ~RenderingPipelineFactory() override;

    /**
     * @brief Creates a Rendering Pipeline
     *
     * @return Unique pointer to the created rendering pipeline instance
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<IRenderingPipeline> override;
  };
} // namespace Dwarf