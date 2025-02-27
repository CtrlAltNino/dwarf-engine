#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/DrawCall/IDrawCallList.h"
#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/Pipelines/IRenderingPipeline.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Rendering/Shader/IShaderFactory.h"
#include "Core/Rendering/DrawCall/IDrawCall.h"
#include <memory>
#include <mutex>

namespace Dwarf
{
  class ForwardRenderer : public IRenderingPipeline
  {
  private:
    std::unique_ptr<IMaterial>        m_IdMaterial;
    std::unique_ptr<IMaterial>        m_GridMaterial;
    std::unique_ptr<IMesh>            m_GridMesh;
    glm::mat4                         m_GridModelMatrix;
    std::shared_ptr<IRendererApi>     m_RendererApi;
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::shared_ptr<IShaderFactory>   m_ShaderFactory;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                   m_ShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>  m_MeshFactory;
    std::shared_ptr<IDrawCallList> m_DrawCallList;

    void
    Setup(glm::ivec2 viewportSize);

  public:
    ForwardRenderer(std::shared_ptr<IRendererApi>     rendererApi,
                    std::shared_ptr<IMaterialFactory> materialFactory,
                    std::shared_ptr<IShaderFactory>   shaderFactory,
                    std::shared_ptr<IShaderSourceCollectionFactory>
                                                  shaderSourceCollectionFactory,
                    std::shared_ptr<IMeshFactory> meshFactory,
                    std::shared_ptr<IDrawCallList> drawCallList);
    ~ForwardRenderer();

    void
    RenderEntity(Entity& entity, ICamera& camera) override;
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