#pragma once

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Base.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"

namespace Dwarf
{
  class RendererApiFactory : public IRendererApiFactory
  {
  private:
    GraphicsApi m_GraphicsApi;

  public:
    RendererApiFactory(GraphicsApi api);

    std::shared_ptr<IRendererApi>
    Create() override;
  };
} // namespace Dwarf