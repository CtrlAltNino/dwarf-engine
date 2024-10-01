#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.h"
#include "Core/Rendering/Framebuffer/IFramebuffer.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Pipelines/IRenderingPipeline.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Rendering/Shader/IShaderFactory.h"
#include <memory>

namespace Dwarf
{
  class ForwardRenderer : public IRenderingPipeline
  {
  private:
    std::unique_ptr<IMaterial>        m_IdMaterial;
    std::shared_ptr<IRendererApi>     m_RendererApi;
    std::shared_ptr<IMaterialFactory> m_MaterialFactory;
    std::shared_ptr<IShaderFactory>   m_ShaderFactory;
    std::shared_ptr<IShaderSourceCollectionFactory>
      m_ShaderSourceCollectionFactory;

    void
    Setup(glm::ivec2 viewportSize);

  public:
    ForwardRenderer(std::shared_ptr<IRendererApi>     rendererApi,
                    std::shared_ptr<IMaterialFactory> materialFactory,
                    std::shared_ptr<IShaderFactory>   shaderFactory,
                    std::shared_ptr<IShaderSourceCollectionFactory>
                      shaderSourceCollectionFactory);
    ~ForwardRenderer();

    void
    RenderEntity(Entity&   entity,
                 glm::mat4 viewMatrix,
                 glm::mat4 projectionMatrix) override;
    void
    RenderScene(IScene&    scene,
                ICamera&   camera,
                glm::ivec2 viewportSize,
                bool       renderGrid) override;

    FramebufferSpecification
    GetSpecification() override;

    void
    RenderIds(IScene& scene, ICamera& camera, glm::ivec2 viewportSize) override;
    // void
    // RenderModelPreview(std::shared_ptr<AssetReference<ModelAsset>>
    // modelAsset,
    //                    std::shared_ptr<Camera>                     camera,
    //                    glm::ivec2 viewportSize, glm::quat rotation) override;
    // void
    // RenderMaterialPreview(
    //   std::shared_ptr<AssetReference<MaterialAsset>> materialAsset,
    //   std::shared_ptr<Camera>                        camera,
    //   glm::ivec2                                     viewportSize,
    //   glm::quat                                      rotation) override;
  };
}