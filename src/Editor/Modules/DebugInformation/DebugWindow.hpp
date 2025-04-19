#pragma once

#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Editor/Modules/IGuiModule.hpp"
#include "Logging/IDwarfLogger.hpp"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief GUI Module to display a window for the asset directory structure.
  class DebugWindow : public IGuiModule
  {
  private:
    std::shared_ptr<IDwarfLogger>   mLogger;
    std::shared_ptr<IAssetDatabase> mAssetDatabase;

  public:
    DebugWindow(std::shared_ptr<IDwarfLogger>   logger,
                std::shared_ptr<IAssetDatabase> assetDatabase);

    DebugWindow(std::shared_ptr<IDwarfLogger>   logger,
                std::shared_ptr<IAssetDatabase> assetDatabase,
                SerializedModule                serializedModule);

    ~DebugWindow() override;

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    /// @brief Executes all pre frame tasks.
    void
    OnUpdate() override;

    auto
    Serialize() -> nlohmann::json override;

    void
    Deserialize(const nlohmann::json& moduleData);
  };
}