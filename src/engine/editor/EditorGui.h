#pragma once

#include<imgui.h>
#include<imgui_impl_glfw.h>
#include<imgui_impl_opengl3.h>
#include<imgui_internal.h>
#include<vector>
#include"../windowing/WindowManager.h"
#include"../editor_modules/IModule.h"

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
            ImGui::ShowDemoWindow();

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
            guiModules.push_back(module);
        }
};