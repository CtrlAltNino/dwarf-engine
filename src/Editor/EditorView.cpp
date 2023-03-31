#include "dpch.h"

#include <nfd.h>

#include "Editor/Editor.h"
#include "Editor/EditorView.h"

#include "Core/Asset/AssetDatabase.h"
#include "Editor/Modules/Asset Browser/AssetBrowserWindow.h"
#include "Editor/Modules/Debug Information/DebugWindow.h"
#include "Editor/Modules/Inspector/InspectorWindow.h"
#include "Editor/Modules/Performance/PerformanceWindow.h"
#include "Editor/Modules/Scene Hierarchy/SceneHierarchyWindow.h"
#include "Editor/Modules/Scene Viewer/SceneViewerWindow.h"

#include "Core/Scene/SceneUtilities.h"

namespace Dwarf {

    EditorView::EditorView(Ref<EditorModel> model) : m_Model(model){
        //ImGui::SetFont
    }

    void EditorView::RenderDockSpace(){
        // If you strip some features of, this demo is pretty much equivalent to calling DockSpaceOverViewport()!
        // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
        // In this specific demo, we are not using DockSpaceOverViewport() because:
        // - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
        // - we allow the host window to have padding (when opt_padding == true)
        // - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
        // TL;DR; this demo is more complicated than what you would normally use.
        // If we removed all the options we are showcasing, this demo would become:
        //     void ShowExampleAppDockSpace()
        //     {
        //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        //     }
        static bool p_open = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(23 / 255.0, 26 / 255.0, 32 / 255.0, 1));
        ImGui::Begin("DockSpace", &p_open, window_flags);
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor();

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("Create new scene");
                if(ImGui::MenuItem("Save scene")){
                    SceneUtilities::SaveScene(m_Model->GetScene());
                    AssetDatabase::Import(m_Model->GetScene()->GetPath());
                    SceneUtilities::SetLastOpenedScene(m_Model->GetScene()->GetPath());
                    Editor::Get().UpdateWindowTitle();
                }
                if(ImGui::MenuItem("Save scene as")){
                    SceneUtilities::SaveSceneDialog(m_Model->GetScene());
                    AssetDatabase::Import(m_Model->GetScene()->GetPath());
                    SceneUtilities::SetLastOpenedScene(m_Model->GetScene()->GetPath());
                    Editor::Get().UpdateWindowTitle();
                }
                if(ImGui::MenuItem("Load scene")){
                    Ref<Scene> loadedScene = SceneUtilities::LoadSceneDialog();
                    if(loadedScene){
                        m_Model->SetScene(loadedScene);
                        SceneUtilities::SetLastOpenedScene(m_Model->GetScene()->GetPath());
                        Editor::Get().UpdateWindowTitle();
                    }
                }
                ImGui::MenuItem("Return to project launcher");
                ImGui::MenuItem("Quit");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Assets"))
            {
                if(ImGui::MenuItem("Create new material")){
                    AssetDatabase::CreateNewMaterialAsset();
                }
                ImGui::MenuItem("Import Assets");
                if(ImGui::MenuItem("Reimport Assets")){
                    AssetDatabase::ReimportAssets();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Scene"))
            {
                if(ImGui::MenuItem("Create empty object")){
                    Entity newEntity = m_Model->GetScene()->CreateEntity("New object");
                    newEntity.AddComponent<MeshRendererComponent>();
                }
                if(ImGui::MenuItem("Create light")){
                    Entity newEntity = m_Model->GetScene()->CreateEntity("New Light");
                    newEntity.AddComponent<LightComponent>();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                if(ImGui::MenuItem("Scene Viewer")){
                    AddWindow(MODULE_TYPE::SCENE_VIEWER);
                }
                if(ImGui::MenuItem("Scene Hierarchy")){
                    AddWindow(MODULE_TYPE::SCENE_GRAPH);
                }
                if(ImGui::MenuItem("Performance")){
                    AddWindow(MODULE_TYPE::PERFORMANCE);
                }
                if(ImGui::MenuItem("Asset Browser")){
                    AddWindow(MODULE_TYPE::ASSET_BROWSER);
                }
                if(ImGui::MenuItem("Inspector")){
                    AddWindow(MODULE_TYPE::INSPECTOR);
                }
                if(ImGui::MenuItem("Debug")){
                    AddWindow(MODULE_TYPE::DEBUG);
                }
                ImGui::MenuItem("Console");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

    void EditorView::Init(){
        AddWindow(MODULE_TYPE::SCENE_GRAPH);
        AddWindow(MODULE_TYPE::INSPECTOR);
        AddWindow(MODULE_TYPE::ASSET_BROWSER);
        AddWindow(MODULE_TYPE::SCENE_VIEWER);
        //AddWindow(MODULE_TYPE::PERFORMANCE);
    }

    void EditorView::OnUpdate(double deltaTime){
        for(int i = 0; i < m_GuiModules.size(); i++){
            m_GuiModules.at(i)->OnUpdate(deltaTime);
        }
    }

    void EditorView::OnImGuiRender(){
        
		ImGui::PushStyleColor(ImGuiCol_Text, COL_TEXT);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, COL_BG_DIM);
		ImGui::PushStyleColor(ImGuiCol_Border, COL_DIM2);

        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, COL_WINDOW_HEADER_FOCUSED);
        ImGui::PushStyleColor(ImGuiCol_TitleBg, COL_WINDOW_HEADER_UNFOCUSED);
        
        ImGui::PushStyleColor(ImGuiCol_TabUnfocused, COL_TAB_UNFOCUSED);
        ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, COL_TAB_UNFOCUSED_ACTIVE);

        ImGui::PushStyleColor(ImGuiCol_Tab, COL_TAB);
        ImGui::PushStyleColor(ImGuiCol_TabActive, COL_TAB_ACTIVE);
        ImGui::PushStyleColor(ImGuiCol_TabHovered, COL_TAB_HOVERED);

        ImGui::PushStyleColor(ImGuiCol_PopupBg, COL_POPUP_BG);

        ImGui::PushStyleColor(ImGuiCol_Button, COL_BUTTON);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, COL_BUTTON_ACTIVE);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, COL_BUTTON_HOVERED);

        ImGui::PushStyleColor(ImGuiCol_Header, COL_BUTTON);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, COL_BUTTON_ACTIVE);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, COL_BUTTON_HOVERED);
        //ImGuiCol_FrameBg
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
        

        // Render default stuff
        RenderDockSpace();
        
        //ImGui::ShowDemoWindow();

        // Render modules
        for(int i = 0; i < m_GuiModules.size(); i++){
            if(m_GuiModules.at(i)->GetWindowClose()){
                RemoveWindow(m_GuiModules.at(i)->GetIndex());
            }else {
                m_GuiModules.at(i)->OnImGuiRender();
            }
        }

        ImGui::PopStyleColor(17);
        ImGui::PopStyleVar(4);
    }

    void EditorView::DockWindowToFocused(std::string windowName){
        //ImGuiViewport* viewport = ImGui::GetMainViewport();


        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        //ImGuiID dockspace_id = ImGui::GetID("DockSpace");


        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;


        ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
        ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
        //ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);


        // split the dockspace into 2 nodes --
        // DockBuilderSplitNode takes in the following args in the following order
        //   window ID to split, direction, fraction (between 0 and 1),
        // the final two setting let's us choose which id we want (which ever one we DON'T set as NULL,
        // will be returned by the function)
        // out_id_at_dir is the id of the node in the direction we specified earlier,
        // out_id_at_opposite_dir is in the opposite direction
        auto dock_id_top = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.2f, nullptr, &dockspace_id);
        auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);
        auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
        auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.15f, nullptr, &dockspace_id);
        //auto dock_id_left2 = ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Down, 0.2f, nullptr, &dock_id_left);
        //auto dock_id_down2 = ImGui::DockBuilderSplitNode(dock_id_down, ImGuiDir_Right, 0.15f, nullptr, &dock_id_down);


        // we now dock our windows into the docking node we made above


        //ImGui::DockBuilderDockWindow("Performance statistics", dock_id_right);
        //ImGui::DockBuilderDockWindow("Window 2", dock_id_right);
        //ImGui::DockBuilderDockWindow("Window 3", dock_id_left);
        //ImGui::DockBuilderDockWindow("Window 4", dock_id_down);
        //ImGui::DockBuilderDockWindow("Window 0", dock_id_top);


        ImGui::DockBuilderFinish(dockspace_id);
    }

    void EditorView::AddWindow(MODULE_TYPE moduleType){
		Ref<GuiModule> guiModule;
		switch(moduleType){
			case MODULE_TYPE::PERFORMANCE:
				guiModule = CreateRef<PerformanceWindow>(PerformanceWindow(this->m_Model, m_GuiModuleIDCount++));
				break;
			case MODULE_TYPE::SCENE_GRAPH:
				guiModule = CreateRef<SceneHierarchyWindow>(SceneHierarchyWindow(this->m_Model, m_GuiModuleIDCount++));
				break;
			case MODULE_TYPE::SCENE_VIEWER:
				guiModule = CreateRef<SceneViewerWindow>(SceneViewerWindow(this->m_Model, m_GuiModuleIDCount++));
				break;
			case MODULE_TYPE::ASSET_BROWSER:
				guiModule = CreateRef<AssetBrowserWindow>(AssetBrowserWindow(this->m_Model, m_GuiModuleIDCount++));
				break;
			case MODULE_TYPE::INSPECTOR:
				guiModule = CreateRef<InspectorWindow>(InspectorWindow(this->m_Model, m_GuiModuleIDCount++));
				break;
			case MODULE_TYPE::DEBUG:
				guiModule = CreateRef<DebugWindow>(DebugWindow(this->m_Model, m_GuiModuleIDCount++));
				break;
			case MODULE_TYPE::CONSOLE: break;
		}

		if(guiModule){
			m_GuiModules.push_back(guiModule);
		}
	}

    void EditorView::RemoveWindow(int index){
		for(int i = 0; i < m_GuiModules.size(); i++){
			if(m_GuiModules[i]->GetIndex() == index){
				m_GuiModules.erase(m_GuiModules.begin()+i);
			}
		}
	}
}