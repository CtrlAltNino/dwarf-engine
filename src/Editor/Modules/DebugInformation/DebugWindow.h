#pragma once
#include "Core/Asset/Database/IAssetDatabase.h"
#include "pch.h"
#include "Editor/Modules/IGuiModule.h"
#include "Editor/EditorModel.h"

namespace Dwarf
{

  /// @brief GUI Module to display a window for the asset directory structure.
  class DebugWindow : public IGuiModule
  {
  private:
    std::shared_ptr<IAssetDatabase> m_AssetDatabase;

  public:
    DebugWindow(int id, std::shared_ptr<IAssetDatabase> assetDatabase);

    /// @brief Renders the module window.
    void
    OnImGuiRender() override;

    /// @brief Executes all pre frame tasks.
    void
    OnUpdate(double deltaTime) override;

    nlohmann::json
    Serialize() const override;

    void
    Deserialize(nlohmann::json moduleData) override;
  };
}