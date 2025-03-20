#pragma once
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/Modules/IGuiModule.h"
#include "pch.h"
#include <boost/serialization/strong_typedef.hpp>

namespace Dwarf
{
  /// @brief GUI Module to display a window for the asset directory structure.
  class DebugWindow : public IGuiModule
  {
  private:
    std::shared_ptr<IAssetDatabase> mAssetDatabase;

  public:
    DebugWindow(std::shared_ptr<IAssetDatabase> assetDatabase);

    DebugWindow(std::shared_ptr<IAssetDatabase> assetDatabase,
                SerializedModule                serializedModule);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    /// @brief Executes all pre frame tasks.
    void
    OnUpdate() override;

    nlohmann::json
    Serialize() override;

    void
    Deserialize(nlohmann::json moduleData);
  };
}