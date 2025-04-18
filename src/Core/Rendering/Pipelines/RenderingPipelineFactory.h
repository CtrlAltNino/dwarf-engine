#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallListFactory.h"
#include "Core/Rendering/DrawCall/DrawCallWorker/IDrawCallWorkerFactory.h"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "Core/Rendering/PingPongBuffer/IPingPongBufferFactory.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include "Core/Rendering/Shader/ShaderRegistry/IShaderRegistry.h"
#include "IRenderingPipelineFactory.h"
#include "Logging/IDwarfLogger.h"

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
      std::shared_ptr<IPingPongBufferFactory> pingPongBufferFactory);

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