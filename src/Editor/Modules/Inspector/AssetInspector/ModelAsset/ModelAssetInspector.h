#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Base.h"
#include "Core/Rendering/PreviewRenderer/ModelPreview/IModelPreview.h"
#include "Editor/Modules/Inspector/AssetInspector/ModelAsset/IModelAssetInspector.h"
#include "Editor/Stats/IEditorStats.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  class ModelAssetInspector : public IModelAssetInspector
  {
  private:
    GraphicsApi                       m_GraphicsApi;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IAssetReimporter> m_AssetReimporter;
    std::shared_ptr<IModelPreview>    m_ModelPreview;
    std::shared_ptr<IInputManager>    m_InputManager;
    std::shared_ptr<IEditorStats>     m_EditorStats;

  public:
    ModelAssetInspector(GraphicsApi                       graphicsApi,
                        std::shared_ptr<IAssetDatabase>   assetDatabase,
                        std::shared_ptr<IAssetReimporter> assetReimporter,
                        std::shared_ptr<IModelPreview>    modelPreview,
                        std::shared_ptr<IInputManager>    inputManager,
                        std::shared_ptr<IEditorStats>     editorStats);
    ~ModelAssetInspector() override = default;

    void
    Render(IAssetReference& asset) override;
  };
}