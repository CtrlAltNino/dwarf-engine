#pragma once

#include "Core/Base.h"
#include "Logging/IDwarfLogger.h"
#include "UI/IImGuiLayerFactory.h"

namespace Dwarf
{
  class ImGuiLayerFactory : public IImGuiLayerFactory
  {
  private:
    std::shared_ptr<IDwarfLogger> m_Logger;
    GraphicsApi                   m_Api;

  public:
    ImGuiLayerFactory(std::shared_ptr<IDwarfLogger> logger, GraphicsApi api);
    ~ImGuiLayerFactory() override;

    std::unique_ptr<IImGuiLayer>
    Create() const override;
  };
} // namespace Dwarf