#include "Editor/Modules/Asset Browser/AssetBrowserWindow.h"
#include "Core/Asset/AssetDatabase.h"
#include "Input/InputManager.h"
#include "Core/Asset/TextureImporter.h"
#include "Core/Rendering/Renderer.h"

namespace Dwarf {

    AssetBrowserWindow::AssetBrowserWindow(Ref<EditorModel> model, int id)
        :GuiModule(model, "Asset Browser", MODULE_TYPE::PERFORMANCE, id),
            m_AssetDirectoryPath(AssetDatabase::GetAssetDirectoryPath()),
            m_CurrentDirectory(m_AssetDirectoryPath){
                m_DirectoryHistory.push_back(m_CurrentDirectory);
                LoadIcons();
    }

    void AssetBrowserWindow::OnUpdate(double deltaTime){
        if(InputManager::GetMouseDown(MOUSE_BUTTON_4)){
            GoBack();
        }else if(InputManager::GetMouseDown(MOUSE_BUTTON_5)){
            GoForward();
        }
    }

    void AssetBrowserWindow::LoadIcons(){
        std::filesystem::path iconPath("data/engine/img/icons/asset browser");
        
        m_DirectoryIcon = TextureImporter::CreateTexture(iconPath / "directoryIcon.png");
            
        m_FBXIcon = TextureImporter::CreateTexture(iconPath / "fbxIcon.png");
        m_OBJIcon = TextureImporter::CreateTexture(iconPath / "objIcon.png");
        
        m_JPGIcon = TextureImporter::CreateTexture(iconPath / "jpgIcon.png");
        m_PNGIcon = TextureImporter::CreateTexture(iconPath / "pngIcon.png");

        m_VertexShaderIcon = TextureImporter::CreateTexture(iconPath / "vertexShaderIcon.png");
        m_TessellationControlShaderIcon = TextureImporter::CreateTexture(iconPath / "tessellationControlShaderIcon.png");
        m_TessellationEvaluationShaderIcon = TextureImporter::CreateTexture(iconPath / "tessellationEvaluationShaderIcon.png");
        m_GeometryShaderIcon = TextureImporter::CreateTexture(iconPath / "geometryShaderIcon.png");
        m_FragmentShaderIcon = TextureImporter::CreateTexture(iconPath / "fragmentShaderIcon.png");
        m_ComputeShaderIcon = TextureImporter::CreateTexture(iconPath / "computeShaderIcon.png");
        m_MetalShaderIcon = TextureImporter::CreateTexture(iconPath / "metalShaderIcon.png");
        m_HLSLShaderIcon = TextureImporter::CreateTexture(iconPath / "hlslIcon.png");

        m_SceneIcon = TextureImporter::CreateTexture(iconPath / "sceneIcon.png");

        m_MaterialIcon = TextureImporter::CreateTexture(iconPath / "materialIcon.png");

        m_UnknownFileIcon = TextureImporter::CreateTexture(iconPath / "unknownFileIcon.png");
    }

    bool CompareDirectoryEntries(std::filesystem::directory_entry d1, std::filesystem::directory_entry d2){
        return d1.path().stem() < d2.path().stem();
    }

    void AssetBrowserWindow::OnImGuiRender(){
        ImGuiWindowFlags window_flags = 0;

        //window_flags |= ImGuiWindowFlags_NoMove;
        //window_flags |= ImGuiWindowFlags_NoResize;
        //window_flags |= ImGuiWindowFlags_NoCollapse;
        //window_flags |= ImGuiWindowFlags_NoTitleBar;
        //window_flags |= ImGuiWindowFlags_MenuBar;

        //static bool b_open = true;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500,500));
        if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, window_flags))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            ImGui::PopStyleVar();
            return;
        }

        ImGui::Dummy(ImGui::GetContentRegionAvail());
        if(ImGui::IsItemClicked()){
            ClearSelection();
        }

        float column = 0;
        float rowOffset = 0;
        float tallestCell = 0;

        std::vector<std::filesystem::directory_entry> directories;
        std::vector<std::filesystem::directory_entry> files;

        //directories.
        for(auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)){
            if(directoryEntry.is_directory()){
                directories.push_back(directoryEntry);
            }else if(directoryEntry.is_regular_file()){
                files.push_back(directoryEntry);
            }
        }

        std::sort(directories.begin(), directories.end(), CompareDirectoryEntries);
        std::sort(files.begin(), files.end(), CompareDirectoryEntries);

        std::vector<std::filesystem::directory_entry> combinedEntries;
        combinedEntries.reserve(directories.size() + files.size());
        combinedEntries.insert(combinedEntries.end(), directories.begin(), directories.end());
        combinedEntries.insert(combinedEntries.end(), files.begin(), files.end());
        
        //for(auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)){
        for(auto& directoryEntry : combinedEntries){
            const auto& path = directoryEntry.path();
            auto relativePath = std::filesystem::relative(path, m_AssetDirectoryPath);

            if(!(directoryEntry.path().has_extension() && directoryEntry.path().extension() == ".meta")){
                float padding = 16.0f * m_IconScale;
                float halfPadding = padding / 2.0f;
                float cellWidth = 64.0f * m_IconScale;
                float textWidth = cellWidth - padding;
                float textHeight = ImGui::CalcTextSize(relativePath.stem().string().c_str(), (const char*)0, false, textWidth).y;
                float cellHeight = cellWidth + textHeight + halfPadding;
                
                if(cellHeight > tallestCell){
                    tallestCell = cellHeight;
                }

                ImVec2 cellMin = {ImGui::GetWindowContentRegionMin().x + column * (cellWidth + halfPadding), ImGui::GetWindowContentRegionMin().y + rowOffset};
                ImGui::SetCursorPos(cellMin);
                
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
                if(m_SelectedAsset == path){
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0,1.0,1.0,0.2));
                }else{
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
                }
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0,1.0,1.0,0.2));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0,1.0,1.0,0.4));
                ImGui::Button("##entry", ImVec2(cellWidth, cellHeight));
                ImGui::PopStyleColor(3);
                ImGui::PopStyleVar();
                
                if(ImGui::IsItemHovered())
                {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    if(ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
                        if(directoryEntry.is_directory()){
                            EnterDirectory(path.filename());
                        }else{
                            // TODO Open file
                        }
                    }else if(ImGui::IsItemClicked()){
                        // TODO Select asset
                        SelectAsset(path);
                    }

                    // TODO Drag Asset
                }

                ImGui::SetCursorPos(ImVec2(cellMin.x + halfPadding, cellMin.y + halfPadding));
                
                static ImTextureID texID;
                if(directoryEntry.is_directory()){
                    texID = (ImTextureID)TextureImporter::GetTexID(m_DirectoryIcon, Renderer::GetAPI());
                }else if(directoryEntry.is_regular_file()){
                    if(directoryEntry.path().has_extension()){
                        if(directoryEntry.path().extension() == ".fbx"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_FBXIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".obj"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_OBJIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".jpg"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_JPGIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".png"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_PNGIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".vert"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_VertexShaderIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".tesc"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_TessellationControlShaderIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".tese"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_TessellationEvaluationShaderIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".geom"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_GeometryShaderIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".frag"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_FragmentShaderIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".comp"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_ComputeShaderIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".metal"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_MetalShaderIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".hlsl"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_HLSLShaderIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".dscene"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_SceneIcon, Renderer::GetAPI());
                        }else if(directoryEntry.path().extension() == ".dmat"){
                            texID = (ImTextureID)TextureImporter::GetTexID(m_MaterialIcon, Renderer::GetAPI());
                        }else{
                            texID = (ImTextureID)TextureImporter::GetTexID(m_UnknownFileIcon, Renderer::GetAPI());
                        }
                    }else{
                        texID = (ImTextureID)TextureImporter::GetTexID(m_UnknownFileIcon, Renderer::GetAPI());
                    }
                }
                ImGui::Image(texID, ImVec2(cellWidth - padding, cellWidth - padding));

                ImGui::SetCursorPos(ImVec2(cellMin.x + halfPadding, cellMin.y + cellWidth));
                ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + textWidth);
                ImGui::TextWrapped(relativePath.stem().string().c_str());
                ImGui::PopTextWrapPos();

                if((ImGui::GetContentRegionAvail().x - cellMin.x - cellWidth) >= cellWidth){
                    column++;
                }else{
                    rowOffset += tallestCell + halfPadding;
                    tallestCell = 0;
                    column = 0;
                }
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }

    void AssetBrowserWindow::GoBack(){
        if(m_HistoryPos > 0){
            m_HistoryPos--;
            m_CurrentDirectory = m_DirectoryHistory[m_HistoryPos];
        }
    }

    void AssetBrowserWindow::GoForward(){
        if(m_HistoryPos < (m_DirectoryHistory.size() - 1)){
            m_HistoryPos++;
            m_CurrentDirectory = m_DirectoryHistory[m_HistoryPos];
        }
    }

    void AssetBrowserWindow::EnterDirectory(std::filesystem::path path){
        m_CurrentDirectory /= path.filename();
        if(m_HistoryPos < (m_DirectoryHistory.size() - 1)){
            for(int i = 0; i < (m_DirectoryHistory.size() - m_HistoryPos); i++){
                m_DirectoryHistory.pop_back();
            }
        }
        m_DirectoryHistory.push_back(m_CurrentDirectory);
        m_HistoryPos = m_DirectoryHistory.size() - 1;
    }

    void AssetBrowserWindow::SelectAsset(std::filesystem::path path){
        m_SelectedAsset = path;
        m_Model->m_Selection.assetPath = m_SelectedAsset;
        m_Model->m_Selection.selectionType = INSPECTOR_SELECTION_TYPE::ASSET;
        // TODO command to inspector
    }

    void AssetBrowserWindow::ClearSelection(){
        m_SelectedAsset = "";
        m_Model->m_Selection.assetPath = "";
        m_Model->m_Selection.selectionType = INSPECTOR_SELECTION_TYPE::NONE;
    }
}