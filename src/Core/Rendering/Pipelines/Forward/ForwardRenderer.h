#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/DrawCall/IDrawCallList.h"
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
  class ForwardRenderer : public IRenderingPipeline
  {
  private:
    std::unique_ptr<IMaterial>        m_IdMaterial;
    std::unique_ptr<IMaterial>        m_GridMaterial;
    std::unique_ptr<IMeshBuffer>      m_GridMeshBuffer;
    glm::mat4                         m_GridModelMatrix;
    std::shared_ptr<IRendererApi>     m_RendererApi;
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::shared_ptr<IShaderRegistry>  m_ShaderRegistry;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        m_ShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>       m_MeshFactory;
    std::shared_ptr<IMeshBufferFactory> m_MeshBufferFactory;
    std::shared_ptr<IDrawCallList>      m_DrawCallList;

    void
    Setup(glm::ivec2 viewportSize);

  public:
    ForwardRenderer(std::shared_ptr<IRendererApi>     rendererApi,
                    std::shared_ptr<IMaterialFactory> materialFactory,
                    std::shared_ptr<IShaderRegistry>  shaderRegistry,
                    std::shared_ptr<IShaderSourceCollectionFactory>
                                                  shaderSourceCollectionFactory,
                    std::shared_ptr<IMeshFactory> meshFactory,
                    std::shared_ptr<IMeshBufferFactory> meshBufferFactory,
                    std::shared_ptr<IDrawCallList>      drawCallList);
    ~ForwardRenderer() override;

    void
    RenderScene(IScene&    scene,
                ICamera&   camera,
                glm::ivec2 viewportSize,
                bool       renderGrid) override;

    FramebufferSpecification
    GetSpecification() override;

    void
    RenderIds(IScene& scene, ICamera& camera, glm::ivec2 viewportSize) override;
  };
}