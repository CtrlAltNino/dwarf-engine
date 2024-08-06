#pragma once

#include "Core/Base.h"
#include "Core/Rendering/RendererApi/IRendererApiFactory.h"

namespace Dwarf
{
  class OpenGLRendererApiFactory : public IRendererApiFactory
  {
  private:
    GraphicsApi m_GraphicsApi;

  public:
    OpenGLRendererApiFactory(GraphicsApi api);
    std::shared_ptr<IRendererApi>
    Create() override;
  };
} // namespace Dwarf