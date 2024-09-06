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
    , m_Selection(selection)
    , m_AssetDatabase(assetDatabase)
    , m_AssetInspector(assetInspector)
    , m_EntityInspector(entityInspector)
  {
  }

  InspectorWindow::InspectorWindow(
    SerializedModule                  serializedModule,
    std::shared_ptr<IEditorSelection> selection,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IAssetInspector>  assetInspector,
    std::shared_ptr<IEntityInspector> entityInspector)
    : IGuiModule(ModuleLabel("Inspector"),
                 ModuleType(MODULE_TYPE::INSPECTOR),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , m_Selection(selection)
    , m_AssetDatabase(assetDatabase)
    , m_AssetInspector(assetInspector)
    , m_EntityInspector(entityInspector)
  {
    Deserialize(serializedModule.t);
  }

  void
  InspectorWindow::OnImGuiRender()
  {
    ImGuiWindowFlags window_flags = 0;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0f, 14.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(200.0f, 0));

    if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      ImGui::PopStyleVar(2);
      return;
    }

    switch (m_Selection->GetSelectionType())
    {
      case CURRENT_SELECTION_TYPE::ASSET:
        {
          // TODO: Render Asset data
          m_AssetInspector->Render(m_Selection->GetSelectedAssetPath());
          break;
        }
      case CURRENT_SELECTION_TYPE::ENTITY:
        {
          // std::vector<Entity>& selectedEntities =
          //   m_Selection->GetSelectedEntities();
          // if (selectedEntities.size() == 1)
          // {
          //   RenderComponents(selectedEntities.at(0));
          // }
          m_EntityInspector->Render(m_Selection->GetSelectedEntities());
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

  void
  InspectorWindow::Deserialize(nlohmann::json moduleData)
  {
    // Deserialize the saved data
  }

  nlohmann::json
  InspectorWindow::Serialize() const
  {
    // TODO:: Serialize the data
    return "";
  }
}