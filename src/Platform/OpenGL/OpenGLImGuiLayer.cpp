#include "pch.h"

#include "Platform/OpenGL/OpenGLImGuiLayer.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <imgui_internal.h>

namespace Dwarf
{
  OpenGLImGuiLayer::OpenGLImGuiLayer(std::shared_ptr<IDwarfLogger> logger,
                                     const ImGuiIniFilePath&       iniFilePath)
    : mLogger(std::move(logger))
    , mIniFilePath((iniFilePath.t / "imgui.ini").string())
  {
    mLogger->LogDebug(Log("Creating OpenGLImGuiLayer", "OpenGLImGuiLayer"));
  }

  OpenGLImGuiLayer::~OpenGLImGuiLayer()
  {
    mLogger->LogDebug(Log("Destroying OpenGLImGuiLayer", "OpenGLImGuiLayer"));
    if ((mWindow != nullptr) && (ImGui::GetCurrentContext() != nullptr))
    {
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplSDL3_Shutdown();
      ImGui::DestroyContext();
    }
  }

  void
  OpenGLImGuiLayer::OnAttach(SDL_Window* window)
  {
    mWindow = window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    io.IniFilename = mIniFilePath.c_str();
    mLogger->LogDebug(
      Log(fmt::format("imgui.ini path set to: {}", mIniFilePath),
          "OpenGLImGuiLayer"));
    io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable
    // Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    io.ConfigWindowsResizeFromEdges = true;
    io.Fonts->AddFontDefault();
    io.FontDefault = io.Fonts->AddFontFromFileTTF(
      "data/engine/fonts/Roboto-Regular.ttf", 15.0F);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform
    // windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
    {
      style.WindowRounding = 0.0F;
      style.Colors[ImGuiCol_WindowBg].w = 1.0F;
    }

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForOpenGL(mWindow, SDL_GL_CreateContext(mWindow));
    ImGui_ImplOpenGL3_Init("#version 410");
  }

  void
  OpenGLImGuiLayer::OnDetach()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
  }

  void
  OpenGLImGuiLayer::Begin()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
  }

  void
  OpenGLImGuiLayer::End()
  {
    const ImGuiIO* io = &ImGui::GetIO();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if ((io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
    {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
    }
  }

  void
  OpenGLImGuiLayer::HandleSDLEvent(SDL_Event* event)
  {
    ImGui_ImplSDL3_ProcessEvent(event);
  }
}