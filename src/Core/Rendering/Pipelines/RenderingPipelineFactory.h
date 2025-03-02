#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/DrawCall/IDrawCallList.h"
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
    std::shared_ptr<IDwarfLogger>     m_Logger;
    std::shared_ptr<IRendererApi>     m_RendererApi;
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::shared_ptr<IShaderRegistry>  m_ShaderRegistry;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        m_ShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>       m_MeshFactory;
    std::shared_ptr<IMeshBufferFactory> m_MeshBufferFactory;
    std::shared_ptr<IDrawCallList>      m_DrawCallList;

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