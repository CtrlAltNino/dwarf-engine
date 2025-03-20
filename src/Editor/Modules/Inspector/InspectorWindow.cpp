#include "Editor/Modules/Inspector/InspectorWindow.h"
#include "Editor/Modules/IGuiModule.h"

namespace Dwarf
{
  InspectorWindow::InspectorWindow(
    std::shared_ptr<IEditorSelection> selection,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetInspector>  assetInspector,
    std::shared_ptr<IEntityInspector> entityInspector)
    : IGuiModule(ModuleLabel("Inspector"),
                 ModuleType(MODULE_TYPE::INSPECTOR),
                 ModuleID(std::make_shared<UUID>()))
    , mSelection(selection)
    , mAssetDatabase(assetDatabase)
    , mAssetInspector(assetInspector)
    , mEntityInspector(entityInspector)
  {
  }

  InspectorWindow::InspectorWindow(
    std::shared_ptr<IEditorSelection> selection,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetInspector>  assetInspector,
    std::shared_ptr<IEntityInspector> entityInspector,
    SerializedModule                  serializedModule)
    : IGuiModule(ModuleLabel("Inspector"),
                 ModuleType(MODULE_TYPE::INSPECTOR),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , mSelection(selection)
    , mAssetDatabase(assetDatabase)
    , mAssetInspector(assetInspector)
    , mEntityInspector(entityInspector)
  {
  }

  void
  InspectorWindow::OnImGuiRender()
  {
    ImGuiWindowFlags window_flags = 0;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0f, 14.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200.0f, 0));

    if (!ImGui::Begin(GetIdentifier().c_str(), &mWindowOpened, window_flags))
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

  nlohmann::json
  InspectorWindow::Serialize()
  {
    nlohmann::json serializedModule;

    serializedModule["id"] = GetUuid()->toString();
    serializedModule["type"] = static_cast<int>(GetModuleType());
    serializedModule["label"] = GetModuleName();

    return serializedModule;
  }
}