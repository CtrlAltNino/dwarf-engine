#pragma once

#include "Core/Asset/AssetReimporter/IAssetReimporter.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Base.h"
#include "Core/Rendering/Material/IO/IMaterialIO.h"
#include "Core/Rendering/PreviewRenderer/MaterialPreview/IMaterialPreview.h"
#include "Editor/Modules/Inspector/AssetInspector/MaterialAsset/IMaterialAssetInspector.h"
#include "Editor/Selection/IEditorSelection.h"
#include "Input/IInputManager.h"

namespace Dwarf
{
  class MaterialAssetInspector : public IMaterialAssetInspector
  {
  private:
    GraphicsApi                       m_GraphicsApi;
    std::shared_ptr<IAssetDatabase>   m_AssetDatabase;
    std::shared_ptr<IAssetReimporter> m_AssetReimporter;
    std::shared_ptr<IMaterialPreview> m_MaterialPreview;
    std::shared_ptr<IInputManager>    m_InputManager;
    std::shared_ptr<IMaterialIO>      m_MaterialIO;

  public:
    MaterialAssetInspector(GraphicsApi                       graphicsApi,
                           std::shared_ptr<IAssetDatabase>   assetDatabase,
                           std::shared_ptr<IAssetReimporter> assetReimporter,
                           std::shared_ptr<IMaterialPreview> materialPreview,
                           std::shared_ptr<IInputManager>    inputManager,
                           std::shared_ptr<IMaterialIO>      materialIO);
    ~MaterialAssetInspector() override = default;

    void
    Render(IAssetReference& asset) override;
  };
}