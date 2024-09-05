#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Base.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Core/Rendering/PreviewRenderer/ModelPreview/IModelPreview.h"
#include "Core/Scene/IO/ISceneIO.h"
#include "Editor/LoadedScene/ILoadedScene.h"
#include "Editor/Modules/Inspector/AssetInspector/IAssetInspector.h"
#include "Editor/Stats/IEditorStats.h"
#include "Input/IInputManager.h"
#include <memory>

namespace Dwarf
{
  class AssetInspector : public IAssetInspector
  {
  public:
    GraphicsApi                       m_GraphicsApi;
    std::shared_ptr<ILoadedScene>     m_LoadedScene;
    std::shared_ptr<ISceneIO>         m_SceneIO;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IMaterialPreview> m_MaterialPreview;
    std::shared_ptr<IModelPreview>    m_ModelPreview;
    std::shared_ptr<IInputManager>    m_InputManager;
    std::shared_ptr<IEditorStats>     m_EditorStats;

    AssetInspector(GraphicsApi                       graphicsApi,
                   std::shared_ptr<ILoadedScene>     loadedScene,
                   std::shared_ptr<ISceneIO>         sceneIO,
                   std::shared_ptr<IAssetDatabase>   assetDatabase,
                   std::shared_ptr<IMaterialPreview> materialPreview,
                   std::shared_ptr<IModelPreview>    modelPreview,
                   std::shared_ptr<IInputManager>    inputManager,
                   std::shared_ptr<IEditorStats>     editorStats);

    ~AssetInspector() override = default;
    void
    Render(const std::filesystem::path& assetPath) const override;

  private:
    // void
    // RenderImpl(std::type_index type, std::shared_ptr<UUID> uid) const
    // override;

    template<typename T>
    void
    RenderAssetInspector(std::shared_ptr<AssetReference<T>> asset) const;
  };
}