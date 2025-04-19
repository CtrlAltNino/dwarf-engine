#pragma once

#include "Core/Base.hpp"
#include "Logging/IDwarfLogger.hpp"
#include "UI/IImGuiLayerFactory.hpp"

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
                      const ImGuiIniFilePath&       iniFilePath);
    ~ImGuiLayerFactory() override;

    /**
     * @brief Creates an ImGuiLayer instance
     *
     * @return Unique pointer to the created imgui layer
     */
    [[nodiscard]] auto
    Create() const -> std::unique_ptr<IImGuiLayer> override;
  };
} // namespace Dwarf