#include"SceneViewerModule.h"

SceneViewerModule::SceneViewerModule(IViewListener *listener, IRenderTexture* renderTexture, int index)
    :IModule(listener, "Scene viewer", MODULE_TYPE::PERFORMANCE, index), renderTexture(renderTexture){ }

void SceneViewerModule::RenderModuleWindow(){
    ImGuiWindowFlags window_flags = 0;

	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	//window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_MenuBar;

    //if (!ImGui::Begin((moduleLabel + "##" + std::to_string(index)).c_str(), NULL, window_flags))
    if (!ImGui::Begin(GetIdentifier().c_str(), &windowOpened, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    //auto app = SSEngine::App::main;
    //uint f_tex = app->getFrameBuffer();
    drawList->AddImage((ImTextureID)renderTexture->GetTexture(),
        pos,
        ImVec2(pos.x + renderTexture->GetResolution().x, pos.y + renderTexture->GetResolution().y),
        ImVec2(0, 1),
        ImVec2(1, 0));

    glm::vec2 resolution = renderTexture->GetResolution();
    ImVec2 imWSize = ImGui::GetWindowSize();
    glm::vec2 windowSize = glm::vec2(imWSize.x, imWSize.y);
    if(windowSize != resolution){
        renderTexture->SetResolution(windowSize.x, windowSize.y);
        renderTexture->SetAspectRatio(windowSize.x/windowSize.y);
    }

    ImGui::End();

    if(!windowOpened)
        viewListener->RemoveWindow(moduleIndex);
}

ImTextureID SceneViewerModule::GetTextureID(){
    return (ImTextureID)renderTexture->GetTexture();
}