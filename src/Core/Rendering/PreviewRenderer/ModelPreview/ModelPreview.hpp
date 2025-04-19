#pragma once

#include "Core/Asset/AssetReference/IAssetReference.hpp"
#include "Core/Asset/Shader/ShaderSourceCollection/IShaderSourceCollectionFactory.hpp"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.hpp"
#include "Core/Rendering/Material/IMaterial.hpp"
#include "Core/Rendering/Material/IMaterialFactory.hpp"
#include "Core/Rendering/Mesh/IMeshFactory.hpp"
#include "Core/Rendering/MeshBuffer/IMeshBufferFactory.hpp"
#include "Core/Rendering/PreviewRenderer/ModelPreview/IModelPreview.hpp"
#include "Core/Scene/Camera/ICameraFactory.hpp"
#include "Input/IInputManager.hpp"
#include "Logging/IDwarfLogger.hpp"

namespace Dwarf
{
  class ModelPreview : public IModelPreview
  {
  private:
    std::shared_ptr<IDwarfLogger>       mLogger;
    std::unique_ptr<IMaterial>          mMaterial;
    std::shared_ptr<IInputManager>      mInputManager;
    std::shared_ptr<IMeshFactory>       mMeshFactory;
    std::shared_ptr<IMeshBufferFactory> mMeshBufferFactory;
    std::unique_ptr<IMeshBuffer>        mPreviewMeshBuffer;
    std::shared_ptr<IShaderSourceCollectionFactory>
      mShaderSourceCollectionFactory;

  public:
    ModelPreview(std::shared_ptr<IDwarfLogger>               logger,
                 std::shared_ptr<IFramebufferFactory>        framebufferFactory,
                 const std::shared_ptr<ICameraFactory>&      cameraFactory,
                 const std::shared_ptr<IRendererApiFactory>& rendererApiFactory,
                 const std::shared_ptr<IMaterialFactory>&    materialFactory,
                 std::shared_ptr<IEditorStats>               editorStats,
                 std::shared_ptr<IInputManager>              inputManager,
                 std::shared_ptr<IMeshFactory>               meshFactory,
                 std::shared_ptr<IMeshBufferFactory>         meshBufferFactory,
                 std::shared_ptr<IShaderSourceCollectionFactory>
                   shaderSourceCollectionFactory);
    ~ModelPreview() override;

    void
    RenderModelPreview(IAssetReference& modelAsset) override;

    void
    FocusModel(ModelAsset& modelAsset) override;
  };
}