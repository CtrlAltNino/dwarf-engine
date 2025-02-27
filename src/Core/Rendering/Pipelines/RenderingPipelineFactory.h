#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/DrawCall/IDrawCallList.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include "Core/Rendering/Shader/IShaderFactory.h"
#include "IRenderingPipelineFactory.h"

namespace Dwarf
{
  class RenderingPipelineFactory : public IRenderingPipelineFactory
  {
  private:
    std::shared_ptr<IRendererApi>     m_RendererApi;
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::shared_ptr<IShaderFactory>   m_ShaderFactory;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                   m_ShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>  m_MeshFactory;
    std::shared_ptr<IDrawCallList> m_DrawCallList;

  public:
    RenderingPipelineFactory(
      std::shared_ptr<IRendererApiFactory> rendererApiFactory,
      std::shared_ptr<IMaterialFactory>    materialFactory,
      std::shared_ptr<IShaderFactory>      shaderFactory,
      std::shared_ptr<IShaderSourceCollectionFactory>
                                     shaderSourceCollectionFactory,
      std::shared_ptr<IMeshFactory>  meshFactory,
      std::shared_ptr<IDrawCallList> drawCallList);

    virtual ~RenderingPipelineFactory() = default;

    std::shared_ptr<IRenderingPipeline>
    Create(PipelineType type) override;
  };
} // namespace Dwarf