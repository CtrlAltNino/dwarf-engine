#pragma once
#include "Core/Asset/Database/IAssetDatabase.h"
#include "pch.h"
#include "Editor/Modules/IGuiModule.h"

namespace Dwarf
{

  /// @brief GUI Module to display a window for the asset directory structure.
  class DebugWindow : public IGuiModule
  {
  private:
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;

  public:
    DebugWindow(std::shared_ptr<IAssetDatabase> assetDatabase);
    DebugWindow(nlohmann::json                  serializedModule,
                std::shared_ptr<IAssetDatabase> assetDatabase);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    /// @brief Executes all pre frame tasks.
    void
    OnUpdate() override;

    nlohmann::json
    Serialize() const override;

    void
    Deserialize(nlohmann::json moduleData) override;
  };
}