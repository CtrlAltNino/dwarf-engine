#include "dpch.h"

#include <nfd.h>

#include "Editor/Editor.h"
#include "Editor/EditorView.h"

#include "Core/Asset/AssetDatabase.h"
#include "Editor/Modules/Asset Browser/AssetBrowserWindow.h"
#include "Editor/Modules/Debug/DebugWindow.h"
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
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &p_open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        else
        {
            //ShowDockingDisabledMessage();
        }

        if (ImGui::BeginMenuBar())
        {
            /*if (ImGui::BeginMenu("Options"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                ImGui::MenuItem("Padding", NULL, &opt_padding);
                ImGui::Separator();

                if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
                if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
                if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
                if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
                ImGui::Separator();

                //if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                //    *p_open = false;
                ImGui::EndMenu();
            }*/
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
            /*HelpMarker(
                "When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!" "\n"
                "- Drag from window title bar or their tab to dock/undock." "\n"
                "- Drag from window menu button (upper-left button) to undock an entire node (all windows)." "\n"
                "- Hold SHIFT to disable docking (if io.ConfigDockingWithShift == false, default)" "\n"
                "- Hold SHIFT to enable docking (if io.ConfigDockingWithShift == true)" "\n"
                "This demo app has nothing to do with enabling docking!" "\n\n"
                "This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window." "\n\n"
                "Read comments in ShowExampleAppDockSpace() for more details.");*/

            ImGui::EndMenuBar();
        }
        ImGui::End();
    }

    void EditorView::Init(){
        AddWindow(MODULE_TYPE::SCENE_GRAPH);
        AddWindow(MODULE_TYPE::INSPECTOR);
        AddWindow(MODULE_TYPE::ASSET_BROWSER);
        //AddWindow(MODULE_TYPE::SCENE_VIEWER);
        //AddWindow(MODULE_TYPE::PERFORMANCE);
    }

    void EditorView::OnUpdate(double deltaTime){
        for(int i = 0; i < m_GuiModules.size(); i++){
            m_GuiModules.at(i)->OnUpdate(deltaTime);
        }
    }

    void EditorView::OnImGuiRender(){
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