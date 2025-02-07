#include "pch.h"
#include "Platform/Metal/MetalImGuiLayer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_metal.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_metal.h>
#include <imgui_impl_sdl2.h>

namespace Dwarf
{
  MetalImGuiLayer::MetalImGuiLayer() {}

  MetalImGuiLayer::~MetalImGuiLayer() {}

  void
  MetalImGuiLayer::OnAttach(SDL_Window* window)
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
    io.ConfigFlags |=
      ImGuiConfigFlags_DockingEnable; // Enable Docking
                                      // io.ConfigFlags |=
                                      // ImGuiConfigFlags_ViewportsEnable; //
                                      // Enable Multi-Viewport / Platform
                                      // Windows
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    io.ConfigWindowsResizeFromEdges = true;
    io.Fonts->AddFontDefault();
    io.FontDefault = io.Fonts->AddFontFromFileTTF(
      "data/engine/fonts/Roboto-Regular.ttf", 15.0f);

    // float fontSize = 18.0f;// *2.0f;
    // io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf",
    // fontSize); io.FontDefault =
    // io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf",
    // fontSize);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform
    // windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      style.WindowRounding = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui::StyleColorsDark();

    SDL_Renderer* renderer = SDL_CreateRenderer(
      m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
      printf("Error creating renderer: %s\n", SDL_GetError());
      return -3;
    }

    CAMetalLayer* layer =
      (__bridge CAMetalLayer*)SDL_RenderGetMetalLayer(renderer);
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    ImGui_ImplMetal_Init(layer.device);
    ImGui_ImplSDL2_InitForMetal(m_Window);

    // id<MTLCommandQueue> commandQueue = [layer.device newCommandQueue];
    // MTLRenderPassDescriptor *renderPassDescriptor = [MTLRenderPassDescriptor
    // new];
  }

  void
  MetalImGuiLayer::OnDetach()
  {
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }

  void
  MetalImGuiLayer::Begin()
  {
    ImGui_ImplMetal_NewFrame();
    // ImGui_ImplSDL2_NewFrame(window); ?????
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
  }

  void
  MetalImGuiLayer::End()
  {
    ImGuiIO* io = &ImGui::GetIO();
    ImGui::Render();
    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData());
    // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
    }

    // glfwSwapBuffers(window);
    // SDL_GL_SwapWindow(window);
  }

  void
  MetalImGuiLayer::HandleSDLEvent(SDL_Event* event)
  {
    ImGui_ImplSDL2_ProcessEvent(event);
  }
}