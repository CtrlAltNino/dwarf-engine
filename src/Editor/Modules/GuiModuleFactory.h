#pragma once

#include "Core/Asset/AssetTypes.h"
#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Asset/Metadata/IAssetMetadata.h"
#include "Core/Rendering/Framebuffer/IFramebufferFactory.h"
#include "Core/Rendering/Material/IMaterialFactory.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/Pipelines/IRenderingPipelineFactory.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include "Core/Rendering/Texture/ITextureFactory.h"
#include "Core/Scene/Camera/ICameraFactory.h"
#include "DI/IDwarfEditorDI.h"
#include "Editor/IEditorSelection.h"
#include "Editor/Modules/IGuiModuleFactory.h"
#include "Editor/Modules/Inspector/AssetInspector/IAssetInspector.h"
#include "Editor/Modules/Inspector/EntityInspector/IEntityInspector.h"
#include "Editor/Stats/IEditorStats.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  class GuiModuleFactory : public IGuiModuleFactory
  {
  private:
    AssetDirectoryPath                         m_AssetDirectoryPath;
    std::shared_ptr<ITextureFactory>           m_TextureFactory;
    std::shared_ptr<IAssetDatabase>            m_AssetDatabase;
    std::shared_ptr<IInputManager>             m_InputManager;
    std::shared_ptr<IEditorSelection>          m_EditorSelection;
    std::shared_ptr<IMaterialIO>               m_MaterialIO;
    std::shared_ptr<IMaterialFactory>          m_MaterialFactory;
    std::shared_ptr<IAssetMetadata>            m_AssetMetadata;
    std::shared_ptr<IMaterialCreator>          m_MaterialCreator;
    std::shared_ptr<IEditorStats>              m_EditorStats;
    std::shared_ptr<IEditor>                   m_Editor;
    std::shared_ptr<ICameraFactory>            m_CameraFactory;
    std::shared_ptr<IFramebufferFactory>       m_FramebufferFactory;
    std::shared_ptr<IRendererApiFactory>       m_RendererApiFactory;
    std::shared_ptr<IRenderingPipelineFactory> m_RenderingPipelineFactory;
    std::shared_ptr<IAssetInspector>           m_AssetInspector;
    std::shared_ptr<IEntityInspector>          m_EntityInspector;

  public:
    explicit GuiModuleFactory(AssetDirectoryPath assetDirectoryPath,
                              std::shared_ptr<ITextureFactory>  textureFactory,
                              std::shared_ptr<IAssetDatabase>   assetDatabase,
                              std::shared_ptr<IInputManager>    inputManager,
                              std::shared_ptr<IEditorSelection> editorSelection,
                              std::shared_ptr<IMaterialIO>      materialIO,
                              std::shared_ptr<IMaterialFactory> materialFactory,
                              std::shared_ptr<IAssetMetadata>   assetMetadata,
                              std::shared_ptr<IMaterialCreator> materialCreator,
                              std::shared_ptr<IEditorStats>     editorStats,
                              std::shared_ptr<IEditor>          editor,
                              std::shared_ptr<IAssetInspector>  assetInspector,
                              std::shared_ptr<IEntityInspector> entityInspector,
                              std::shared_ptr<ICameraFactory>   cameraFactory);

    std::shared_ptr<IGuiModule>
    CreateGuiModule(MODULE_TYPE type) override;

    std::shared_ptr<IGuiModule>
    CreateGuiModule(nlohmann::json serializedModule) override;
  };
} // namespace Dwarf