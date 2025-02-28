#include "RenderingPipelineFactory.h"

#include "Core/Rendering/Pipelines/Forward/ForwardRenderer.h"
// #include "Core/Rendering/Pipelines/Deferred/DeferredRenderer.h"

namespace Dwarf
{
  RenderingPipelineFactory::RenderingPipelineFactory(
    std::shared_ptr<IRendererApiFactory> rendererApiFactory,
    std::shared_ptr<IMaterialFactory>    materialFactory,
    std::shared_ptr<IShaderFactory>      shaderFactory,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        shaderSourceCollectionFactory,
    std::shared_ptr<IMeshFactory>       meshFactory,
    std::shared_ptr<IMeshBufferFactory> meshBufferFactory,
    std::shared_ptr<IDrawCallList>      drawCallList)
    : m_RendererApi(rendererApiFactory->Create())
    , m_MaterialFactory(materialFactory)
    , m_ShaderFactory(shaderFactory)
    , m_ShaderSourceCollectionFactory(shaderSourceCollectionFactory)
    , m_MeshFactory(meshFactory)
    , m_MeshBufferFactory(meshBufferFactory)
    , m_DrawCallList(drawCallList)
  {
  }

  std::shared_ptr<IRenderingPipeline>
  RenderingPipelineFactory::Create(PipelineType type)
  {
    switch (type)
    {
      case PipelineType::Forward:
        return std::make_shared<ForwardRenderer>(
          m_RendererApi,
          m_MaterialFactory,
          m_ShaderFactory,
          m_ShaderSourceCollectionFactory,
          m_MeshFactory,
          m_MeshBufferFactory,
          m_DrawCallList);
      // case PipelineType::Deferred: return
      // std::make_shared<DeferredRenderer>();
      default: return nullptr;
    }
  }
} // namespace Dwarf