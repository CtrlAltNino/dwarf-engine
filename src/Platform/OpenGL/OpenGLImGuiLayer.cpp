#include "dpch.h"

#include "Platform/OpenGL/OpenGLImguiLayer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>

namespace Dwarf
{
	OpenGLImGuiLayer::OpenGLImGuiLayer() {}

	OpenGLImGuiLayer::~OpenGLImGuiLayer()
	{
	}

	void OpenGLImGuiLayer::OnAttach(void *window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
														  // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		// io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		io.ConfigWindowsResizeFromEdges = true;
		io.Fonts->AddFontDefault();
		io.FontDefault = io.Fonts->AddFontFromFileTTF("data/engine/fonts/Roboto-Regular.ttf", 15.0f);

		// float fontSize = 18.0f;// *2.0f;
		// io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", fontSize);
		// io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", fontSize);

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle &style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// SetDarkThemeColors();
		ImGui::StyleColorsDark();

		// Application& app = Application::Get();
		// SDL_Window* window = static_cast<SDL_Window*>(app.GetWindow().GetNativeWindow());
		SDL_Window *sdlWindow = (SDL_Window *)window;

		// context = SDL_GL_CreateContext(window);

		// Setup Platform/Renderer bindings
		// ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplSDL2_InitForOpenGL(sdlWindow, SDL_GL_CreateContext(sdlWindow));
		ImGui_ImplOpenGL3_Init("#version 410");

		// ImGui::CreateContext();

		// ImGui_ImplGlfw_InitForOpenGL(window, true);
		// ImGui_ImplSDL2_InitForOpenGL(window, context);
		// ImGui_ImplOpenGL3_Init("#version 130");
	}

	void OpenGLImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void OpenGLImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
	}

	void OpenGLImGuiLayer::End()
	{
		ImGuiIO *io = &ImGui::GetIO();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		// glfwSwapBuffers(window);
		// SDL_GL_SwapWindow(window);
	}

	void OpenGLImGuiLayer::HandleSDLEvent(SDL_Event *event)
	{
		ImGui_ImplSDL2_ProcessEvent(event);
	}
}