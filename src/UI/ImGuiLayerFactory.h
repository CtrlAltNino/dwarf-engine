#pragma once

#include "Core/Base.h"
#include "Logging/IDwarfLogger.h"
#include "UI/IImGuiLayerFactory.h"

namespace Dwarf
{
  class ImGuiLayerFactory : public IImGuiLayerFactory
  {
  private:
    std::shared_ptr<IDwarfLogger> mLogger;
    GraphicsApi                   mApi;
    ImGuiIniFilePath              mIniFilePath;

  public:
    ImGuiLayerFactory(std::shared_ptr<IDwarfLogger> logger,
                      GraphicsApi                   api,
                      ImGuiIniFilePath              iniFilePath);
    ~ImGuiLayerFactory() override;

    std::unique_ptr<IImGuiLayer>
    Create() const override;
  };
} // namespace Dwarf