#include "pch.h"
#include "Platform/OpenGL/OpenGLImGuiLayer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

namespace Dwarf
{
  OpenGLImGuiLayer::~OpenGLImGuiLayer()
  {
    std::cout << "Shutting down ImGui" << std::endl;
    if (m_Window && ImGui::GetCurrentContext())
    {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplSDL2_Shutdown();
      ImGui::DestroyContext();
    }
  }
  void
  OpenGLImGuiLayer::OnAttach(SDL_Window* window)
  {
    m_Window = window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable
    // Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    io.ConfigWindowsResizeFromEdges = true;
    io.Fonts->AddFontDefault();
    io.FontDefault = io.Fonts->AddFontFromFileTTF(
      "data/engine/fonts/Roboto-Regular.ttf", 15.0f);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform
    // windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_Window, SDL_GL_CreateContext(m_Window));
    ImGui_ImplOpenGL3_Init("#version 410");
  }

  void
  OpenGLImGuiLayer::OnDetach()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }

  void
  OpenGLImGuiLayer::Begin()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
  }

  void
  OpenGLImGuiLayer::End()
  {
    const ImGuiIO* io = &ImGui::GetIO();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
    }
  }

  void
  OpenGLImGuiLayer::HandleSDLEvent(SDL_Event* event)
  {
    ImGui_ImplSDL2_ProcessEvent(event);
  }
}