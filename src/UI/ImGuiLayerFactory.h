#pragma once

#include "Core/Base.h"
#include "UI/IImGuiLayerFactory.h"

namespace Dwarf
{
  class ImGuiLayerFactory : public IImGuiLayerFactory
  {
  private:
    GraphicsApi m_Api;

  public:
    ImGuiLayerFactory(GraphicsApi api);

    std::shared_ptr<IImGuiLayer>
    Create() override;
  };
} // namespace Dwarf