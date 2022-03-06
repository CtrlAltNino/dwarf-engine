#pragma once

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include<imgui_internal.h>
#include<vector>
#include"../windowing/WindowManager.h"
#include"../editor_modules/IModule.h"
#include"../editor_modules/PerformanceModule.h"
#include"Editor.h"

//enum class EDITOR_MODULE_REGISTRY {PERFORMANCE_MODULE}

class EditorGui{
    private:
        static inline std::vector<IModule*> guiModules;
        
        static void InitGUI_OpenGL(){
            ImGui::CreateContext();
            io = &ImGui::GetIO();
            
            io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;

            ImGui_ImplGlfw_InitForOpenGL(WindowManager::GetWindow_OpenGL(), true);
            ImGui_ImplOpenGL3_Init("#version 130");

            ImGui::StyleColorsDark();
            io->ConfigWindowsResizeFromEdges = false;
        }

        static void RenderDockSpace(){
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
            ImGui::Begin("DockSpace Demo", &p_open, window_flags);
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
                    ImGui::MenuItem("Save scene");
                    ImGui::MenuItem("Load scene");
                    ImGui::MenuItem("Return to project launcher");
                    ImGui::MenuItem("Quit");
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Assets"))
                {
                    ImGui::MenuItem("Create asset");
                    ImGui::MenuItem("Import Asset");
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Scene"))
                {
                    ImGui::MenuItem("Pause/Unpause scene");
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Window"))
                {
                    ImGui::MenuItem("Scene viewer");
                    ImGui::MenuItem("Scene graph");
                    if(ImGui::MenuItem("Performance statistics")){
                        //AddWindow(new PerformanceModule(&(editorReference->GetScene()->deltaTime)));
                    }
                    ImGui::MenuItem("Asset browser");
                    ImGui::MenuItem("Inspector");
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
    public:
        static inline ImGuiIO* io;
        
        static void InitGUI(){
            switch(WindowManager::GetActiveGraphicsApi()){
                case GraphicsApi::OpenGL:
                    InitGUI_OpenGL();
                    break;
                case GraphicsApi::D3D11:
                    break;
                case GraphicsApi::D3D12:
                    break;
                case GraphicsApi::Vulkan:
                    break;
            }
        }

        static void StartFrame(){
            switch(WindowManager::GetActiveGraphicsApi()){
                case GraphicsApi::OpenGL:
                    ImGui_ImplOpenGL3_NewFrame();
                    ImGui_ImplGlfw_NewFrame();
                    ImGui::NewFrame();
                    break;
                case GraphicsApi::D3D11:
                    break;
                case GraphicsApi::D3D12:
                    break;
                case GraphicsApi::Vulkan:
                    break;
            }
        }

        static void RenderGUI(){
            // Render default stuff
            RenderDockSpace();
            
            
            //ImGui::ShowDemoWindow();

            // Render modules
            for(int i = 0; i < guiModules.size(); i++){
                std::cout << "Now rendering module" << std::endl;
                guiModules.at(i)->RenderModuleWindow();
            }
        }

        static void EndFrame(){
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
            }
        }

        static void AddWindow(IModule *module){
            if(module != nullptr){
                guiModules.push_back(module);
            }
        }
};