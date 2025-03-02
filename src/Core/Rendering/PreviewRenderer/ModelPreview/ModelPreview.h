#pragma once

#include "Core/Asset/AssetReference/IAssetReference.h"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Core/Rendering/Material/IMaterial.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Mesh/IMeshFactory.h"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.h"
#include "Core/Rendering/PreviewRenderer/ModelPreview/IModelPreview.h"
#include "Core/Scene/Camera/ICameraFactory.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  class ModelPreview : public IModelPreview
  {
  private:
    std::shared_ptr<IMaterialFactory>   m_MaterialFactory;
    std::unique_ptr<IMaterial>          m_Material;
    std::shared_ptr<IInputManager>      m_InputManager;
    std::shared_ptr<IMeshFactory>       m_MeshFactory;
    std::shared_ptr<IMeshBufferFactory> m_MeshBufferFactory;
    std::unique_ptr<IMeshBuffer>        m_PreviewMeshBuffer;

  public:
    ModelPreview(std::shared_ptr<IFramebufferFactory> framebufferFactory,
                 std::shared_ptr<ICameraFactory>      cameraFactory,
                 std::shared_ptr<IRendererApiFactory> rendererApiFactory,
                 std::shared_ptr<IMaterialFactory>    materialFactory,
                 std::shared_ptr<IEditorStats>        editorStats,
                 std::shared_ptr<IInputManager>       inputManager,
                 std::shared_ptr<IMeshFactory>        meshFactory,
                 std::shared_ptr<IMeshBufferFactory>  meshBufferFactory);

    void
    RenderModelPreview(IAssetReference& modelAsset) override;

    void
    FocusModel(ModelAsset& modelAsset) override;
  };
}