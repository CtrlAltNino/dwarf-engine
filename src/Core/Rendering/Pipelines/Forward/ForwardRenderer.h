#pragma once

#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/DrawCall/DrawCallList/IDrawCallList.h"
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
    std::unique_ptr<IMaterial>        mIdMaterial;
    std::unique_ptr<IMaterial>        mGridMaterial;
    std::unique_ptr<IMeshBuffer>      mGridMeshBuffer;
    glm::mat4                         mGridModelMatrix;
    std::shared_ptr<IRendererApi>     mRendererApi;
    std::shared_ptr<IMaterialFactory> mMaterialFactory;
    std::shared_ptr<IShaderRegistry>  mShaderRegistry;
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        mShaderSourceCollectionFactory;
    std::shared_ptr<IMeshFactory>       mMeshFactory;
    std::shared_ptr<IMeshBufferFactory> mMeshBufferFactory;
    std::shared_ptr<IDrawCallList>      mDrawCallList;

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