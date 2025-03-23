#include "Editor/Modules/Inspector/InspectorWindow.h"
#include "Editor/Modules/IGuiModule.h"

namespace Dwarf
{
  InspectorWindow::InspectorWindow(
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<IEditorSelection> selection,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetInspector>  assetInspector,
    std::shared_ptr<IEntityInspector> entityInspector)
    : IGuiModule(ModuleLabel("Inspector"),
                 ModuleType(MODULE_TYPE::INSPECTOR),
                 ModuleID(std::make_shared<UUID>()))
    , mLogger(std::move(logger))
    , mSelection(std::move(selection))
    , mAssetDatabase(std::move(assetDatabase))
    , mAssetInspector(std::move(assetInspector))
    , mEntityInspector(std::move(entityInspector))
  {
  }

  InspectorWindow::InspectorWindow(
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<IEditorSelection> selection,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetInspector>  assetInspector,
    std::shared_ptr<IEntityInspector> entityInspector,
    SerializedModule                  serializedModule)
    : IGuiModule(ModuleLabel("Inspector"),
                 ModuleType(MODULE_TYPE::INSPECTOR),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , mLogger(std::move(logger))
    , mSelection(std::move(selection))
    , mAssetDatabase(std::move(assetDatabase))
    , mAssetInspector(std::move(assetInspector))
    , mEntityInspector(std::move(entityInspector))
  {
  }

  void
  InspectorWindow::OnImGuiRender()
  {
    ImGuiWindowFlags windowFlags = 0;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0F, 14.0F));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200.0F, 0));

    if (!ImGui::Begin(GetIdentifier().c_str(), &mWindowOpened, windowFlags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      ImGui::PopStyleVar(2);
      return;
    }

    switch (mSelection->GetSelectionType())
    {
      case CURRENT_SELECTION_TYPE::ASSET:
        {
          mAssetInspector->Render();
          break;
        }
      case CURRENT_SELECTION_TYPE::ENTITY:
        {
          mEntityInspector->Render(mSelection->GetSelectedEntities());
          break;
        }
      case CURRENT_SELECTION_TYPE::NONE: break;
    }

    ImGui::End();

    ImGui::PopStyleVar(2);
  }

  void
  InspectorWindow::OnUpdate()
  {
    // Place Update code here (Before rendering)
  }

  auto
  InspectorWindow::Serialize() -> nlohmann::json
  {
    nlohmann::json serializedModule;

    serializedModule["id"] = GetUuid()->toString();
    serializedModule["type"] = static_cast<int>(GetModuleType());
    serializedModule["label"] = GetModuleName();

    return serializedModule;
  }
}