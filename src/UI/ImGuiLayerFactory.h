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
    ~ImGuiLayerFactory() override;

    std::shared_ptr<IImGuiLayer>
    Create() const override;
  };
} // namespace Dwarf