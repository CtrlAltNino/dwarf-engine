#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Base.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"
#include <boost/di/extension/injections/lazy.hpp>

namespace Dwarf
{
  class RendererApiFactory : public IRendererApiFactory
  {
  private:
    GraphicsApi                     m_GraphicsApi;
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;

  public:
    RendererApiFactory(GraphicsApi                     api,
                       std::shared_ptr<IAssetDatabase> assetDatabase);

    std::shared_ptr<IRendererApi>
    Create() override;
  };
} // namespace Dwarf