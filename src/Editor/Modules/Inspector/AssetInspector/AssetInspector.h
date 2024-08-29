#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Base.h"
#include "Editor/Modules/Inspector/AssetInspector/IAssetInspector.h"
#include "Editor/EditorModel.h"

namespace Dwarf
{
  class AssetInspector : public IAssetInspector
  {
  public:
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;
    GraphicsApi                     m_GraphicsApi;

    AssetInspector(std::shared_ptr<IAssetDatabase> assetDatabase,
                   GraphicsApi                     graphicsApi);

  private:
    void
    RenderImpl(std::type_index type, std::shared_ptr<UUID> uid) const override;

    template<typename T>
    void
    RenderAssetInspector(std::shared_ptr<AssetReference<T>> asset);
  };
}