#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallList.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
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
    std::shared_ptr<IMeshFactory>       mMeshFactory;
    std::shared_ptr<IMeshBufferFactory> mMeshBufferFactory;
    std::shared_ptr<IDrawCallList>      mDrawCallList;

  public:
    RenderingPipelineFactory(
      std::shared_ptr<IDwarfLogger>        logger,
      std::shared_ptr<IRendererApiFactory> rendererApiFactory,
      std::shared_ptr<IMaterialFactory>    materialFactory,
      std::shared_ptr<IShaderRegistry>     shaderRegistry,
      std::shared_ptr<IShaderSourceCollectionFactory>
                                          shaderSourceCollectionFactory,
      std::shared_ptr<IMeshFactory>       meshFactory,
      std::shared_ptr<IMeshBufferFactory> meshBufferFactory,
      std::shared_ptr<IDrawCallList>      drawCallList);

    ~RenderingPipelineFactory() override;

    std::shared_ptr<IRenderingPipeline>
    Create(PipelineType type) override;
  };
} // namespace Dwarf