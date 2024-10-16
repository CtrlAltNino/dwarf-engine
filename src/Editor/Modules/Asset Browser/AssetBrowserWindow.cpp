#include "Editor/Modules/Asset Browser/AssetBrowserWindow.h"
#include "Input/InputManager.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Scene/SceneUtilities.h"
#include <imgui_internal.h>

namespace Dwarf
{

  AssetBrowserWindow::AssetBrowserWindow(std::shared_ptr<EditorModel> model,
                                         int                          id)
    : GuiModule(model, "Asset Browser", MODULE_TYPE::PERFORMANCE, id)
    , m_CurrentDirectory(m_AssetDirectoryPath)
  {
    m_DirectoryHistory.push_back(m_CurrentDirectory);
    LoadIcons();
  }

  void
  AssetBrowserWindow::SetupDockspace(ImGuiID id)
  {
    ImGui::DockBuilderRemoveNode(
      id); // Clear any preexisting layouts associated with the ID we just chose
    ImGui::DockBuilderAddNode(id, ImGuiDockNodeFlags_HiddenTabBar);

    footerID =
      ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 0.07f, nullptr, &id);
    ImGuiID folderStructureID =
      ImGui::DockBuilderSplitNode(id, ImGuiDir_Left, 0.3f, nullptr, &id);
    ImGuiID folderContent = ImGui::DockBuilderSplitNode(
      folderStructureID, ImGuiDir_Right, 0.7f, nullptr, &folderStructureID);

    // 6. Add windows to each docking space:
    ImGui::DockBuilderDockWindow("FolderStructure", folderStructureID);
    ImGui::DockBuilderDockWindow("Footer", footerID);
    ImGui::DockBuilderDockWindow("FolderContent", folderContent);

    // 7. We're done setting up our docking configuration:
    ImGui::DockBuilderFinish(id);
  }

  void
  AssetBrowserWindow::OnUpdate(double deltaTime)
  {
    if (InputManager::GetMouseDown(MOUSE_BUTTON::MOUSE_BUTTON_4))
    {
      GoBack();
    }
    else if (InputManager::GetMouseDown(MOUSE_BUTTON::MOUSE_BUTTON_5))
    {
      GoForward();
    }
  }

  void
  AssetBrowserWindow::LoadIcons()
  {
    std::filesystem::path iconPath("data/engine/img/icons/asset browser");

    m_DirectoryIcon = TextureCreator::FromPath(iconPath / "directoryIcon.png");

    m_FBXIcon = TextureCreator::FromPath(iconPath / "fbxIcon.png");
    m_OBJIcon = TextureCreator::FromPath(iconPath / "objIcon.png");

    m_JPGIcon = TextureCreator::FromPath(iconPath / "jpgIcon.png");
    m_PNGIcon = TextureCreator::FromPath(iconPath / "pngIcon.png");

    m_VertexShaderIcon =
      TextureCreator::FromPath(iconPath / "vertexShaderIcon.png");
    m_TessellationControlShaderIcon =
      TextureCreator::FromPath(iconPath / "tessellationControlShaderIcon.png");
    m_TessellationEvaluationShaderIcon = TextureCreator::FromPath(
      iconPath / "tessellationEvaluationShaderIcon.png");
    m_GeometryShaderIcon =
      TextureCreator::FromPath(iconPath / "geometryShaderIcon.png");
    m_FragmentShaderIcon =
      TextureCreator::FromPath(iconPath / "fragmentShaderIcon.png");
    m_ComputeShaderIcon =
      TextureCreator::FromPath(iconPath / "computeShaderIcon.png");
    m_HLSLShaderIcon = TextureCreator::FromPath(iconPath / "hlslIcon.png");

    m_SceneIcon = TextureCreator::FromPath(iconPath / "sceneIcon.png");

    m_MaterialIcon = TextureCreator::FromPath(iconPath / "materialIcon.png");

    m_UnknownFileIcon =
      TextureCreator::FromPath(iconPath / "unknownFileIcon.png");
  }

  bool
  CompareDirectoryEntries(std::filesystem::directory_entry const& d1,
                          std::filesystem::directory_entry const& d2)
  {
    return d1.path().stem() < d2.path().stem();
  }

  void
  AssetBrowserWindow::OnImGuiRender()
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500, 500));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (!ImGui::Begin(GetIdentifier().c_str(), &m_WindowOpened, 0))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      ImGui::PopStyleVar(2);
      return;
    }
    ImGui::PopStyleVar(2);

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    dockspace_flags |= ImGuiDockNodeFlags_HiddenTabBar;
    ImGuiID dockspace_id = ImGui::GetID("AssetBrowserDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    if (firstFrame)
    {
      firstFrame = false;
      SetupDockspace(dockspace_id);
    }

    RenderFolderStructure();
    RenderFolderContent();
    RenderFooter();

    ImGui::End();
  }

  void
  AssetBrowserWindow::RenderDirectoryLevel(
    std::filesystem::path const& directory)
  {
    for (auto& directoryEntry : std::filesystem::directory_iterator(directory))
    {
      if (directoryEntry.is_directory())
      {
        if (ImGui::CollapsingHeader(
              directoryEntry.path().stem().string().c_str()))
        {
          ImGui::Indent(8.0f);
          RenderDirectoryLevel(directoryEntry.path());
          ImGui::Unindent(8.0f);
        }

        if (ImGui::IsItemClicked())
        {
          m_CurrentDirectory = directoryEntry.path();
        }
      }
    }
  }

  void
  AssetBrowserWindow::RenderFolderStructure()
  {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(150, 150));
    ImGui::Begin("FolderStructure", nullptr, window_flags);
    if (ImGui::CollapsingHeader("Assets"))
    {
      if (ImGui::IsItemClicked())
      {
        m_CurrentDirectory = AssetDatabase::GetAssetDirectoryPath();
      }
      ImGui::Indent(8.0f);
      RenderDirectoryLevel(AssetDatabase::GetAssetDirectoryPath());
      ImGui::Unindent(8.0f);
    }
    ImGui::End();
    ImGui::PopStyleVar();
  }

  void
  AssetBrowserWindow::RenderFooter()
  {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500, 500));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
    ImGui::Begin("Footer", nullptr, window_flags);
    if (ImGui::Button("<", ImVec2(20, 20)))
    {
      GoBack();
    }
    ImGui::SameLine(0.0f, 5.0f);
    if (ImGui::Button(">", ImVec2(20, 20)))
    {
      GoForward();
    }
    ImGui::SameLine(0.0f, 5.0f);
    ImGui::SliderFloat("Size", &m_IconScale, 1.0f, 2.0f);
    ImGui::End();
    ImGui::PopStyleVar(2);
  }

  void
  AssetBrowserWindow::RenderFolderContent()
  {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;

    ImGui::Begin("FolderContent", nullptr, window_flags);

    ImGui::Dummy(ImGui::GetContentRegionAvail());

    if (ImGui::BeginPopupContextItem(
          "Backdrop")) // <-- use last item id as popup id
    {
      if (ImGui::BeginMenu("New"))
      {
        if (ImGui::BeginMenu("Material"))
        {
          // TODO: Make this moddable
          if (ImGui::MenuItem("Default"))
          {
            AssetDatabase::CreateNewMaterialAsset(m_CurrentDirectory);
          }
          ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Shader"))
        {
          if (ImGui::MenuItem("Vertex"))
          {
            FileHandler::Copy(Shader::GetDefaultShaderPath() / "default.vert",
                              m_CurrentDirectory / "New vertex shader.vert");
          }

          if (ImGui::MenuItem("Fragment"))
          {
            FileHandler::Copy(Shader::GetDefaultShaderPath() / "default.frag",
                              m_CurrentDirectory / "New fragment shader.frag");
          }

          ImGui::EndMenu();
        }

        ImGui::MenuItem("Scene");
        ImGui::EndMenu();
      }

      if (ImGui::MenuItem("Create Folder"))
      {
        FileHandler::CreateDirectory(m_CurrentDirectory / "New Folder");
      }

      if (ImGui::MenuItem("Paste"))
      {
        if (std::filesystem::exists(m_CopyPathBuffer))
        {
          FileHandler::Copy(m_CopyPathBuffer, m_CurrentDirectory);
        }
      }

      if (ImGui::MenuItem("Open in file browser"))
      {
        FileHandler::OpenPathInFileBrowser(m_CurrentDirectory);
      }
      ImGui::EndPopup();
    }

    float column = 0;
    float rowOffset = 0;
    float tallestCell = 0;

    std::vector<std::filesystem::directory_entry> directories;
    std::vector<std::filesystem::directory_entry> files;

    // directories.
    for (auto& directoryEntry :
         std::filesystem::directory_iterator(m_CurrentDirectory))
    {
      if (directoryEntry.is_directory())
      {
        directories.push_back(directoryEntry);
      }
      else if (directoryEntry.is_regular_file())
      {
        files.push_back(directoryEntry);
      }
    }

    std::ranges::sort(
      directories.begin(), directories.end(), CompareDirectoryEntries);
    std::ranges::sort(files.begin(), files.end(), CompareDirectoryEntries);

    std::vector<std::filesystem::directory_entry> combinedEntries;
    combinedEntries.reserve(directories.size() + files.size());
    combinedEntries.insert(
      combinedEntries.end(), directories.begin(), directories.end());
    combinedEntries.insert(combinedEntries.end(), files.begin(), files.end());

    for (auto const& directoryEntry : combinedEntries)
    {
      const auto& path = directoryEntry.path();
      auto relativePath = std::filesystem::relative(path, m_AssetDirectoryPath);

      if (!(directoryEntry.path().has_extension() &&
            directoryEntry.path().extension() ==
              AssetMetaData::META_DATA_EXTENSION))
      {
        float padding = 16.0f * m_IconScale;
        float halfPadding = padding / 2.0f;
        float cellWidth = 64.0f * m_IconScale;
        float textWidth = cellWidth - padding;
        float textHeight =
          ImGui::CalcTextSize(
            relativePath.stem().string().c_str(), nullptr, false, textWidth)
            .y;
        float cellHeight = cellWidth + textHeight + halfPadding;

        if (cellHeight > tallestCell)
        {
          tallestCell = cellHeight;
        }

        ImVec2 cellMin = { ImGui::GetWindowContentRegionMin().x +
                             column * (cellWidth + halfPadding),
                           ImGui::GetWindowContentRegionMin().y + rowOffset };
        ImGui::SetCursorPos(cellMin);

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        if (m_SelectedAsset == path)
        {
          ImGui::PushStyleColor(ImGuiCol_Button,
                                ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
        }
        else
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        }
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4(1.0f, 1.0f, 1.0f, 0.4f));
        ImGui::Button("##entry", ImVec2(cellWidth, cellHeight));
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();

        if (ImGui::IsItemHovered())
        {
          ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
          if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
          {
            OpenPath(directoryEntry);
          }
          else if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
          {
            SelectAsset(path);
          }

          // TODO: Drag Asset
        }
        else
        {
          if (m_SelectedAsset == directoryEntry.path() &&
              ImGui::IsWindowHovered() &&
              ImGui::IsMouseClicked(ImGuiMouseButton_Left))
          {
            ClearSelection();
          }
        }

        if (ImGui::BeginPopupContextItem(
              directoryEntry.path().string().c_str())) // <-- use last item id
                                                       // as popup id
        {
          if (ImGui::MenuItem("Open"))
          {
            OpenPath(directoryEntry);
          }
          else if (ImGui::MenuItem("Copy"))
          {
            m_CopyPathBuffer = path;
          }
          else if (ImGui::MenuItem("Duplicate"))
          {
            FileHandler::Duplicate(path);
          }
          else if (ImGui::MenuItem("Rename"))
          {
            m_RenamePathBuffer = path;
            SetRenameBuffer(path);
            m_OpenRename = true;
          }
          if (ImGui::MenuItem("Open in file browser"))
          {
            if (directoryEntry.is_directory())
            {
              FileHandler::OpenPathInFileBrowser(directoryEntry.path());
            }
            else
            {
              FileHandler::OpenPathInFileBrowser(m_CurrentDirectory);
            }
          }
          else if (ImGui::MenuItem("Delete"))
          {
            // TODO: Confirmation modal
            if (AssetDatabase::Exists(path))
            {
              AssetDatabase::Remove(path);
              AssetMetaData::RemoveMetaData(path);
            }
            FileHandler::Delete(path);
          }
          ImGui::EndPopup();
        }

        ImGui::SetCursorPos(
          ImVec2(cellMin.x + halfPadding, cellMin.y + halfPadding));

        static ImTextureID texID;
        if (directoryEntry.is_directory())
        {
          texID = (ImTextureID)m_DirectoryIcon->GetTextureID();
        }
        else if (directoryEntry.is_regular_file())
        {
          if (directoryEntry.path().has_extension())
          {
            if (directoryEntry.path().extension() == ".fbx")
            {
              texID = (ImTextureID)m_FBXIcon->GetTextureID();
            }
            else if (directoryEntry.path().extension() == ".obj")
            {
              texID = (ImTextureID)m_OBJIcon->GetTextureID();
            }
            else if (directoryEntry.path().extension() == ".jpg")
            {
              texID = (ImTextureID)m_JPGIcon->GetTextureID();
            }
            else if (directoryEntry.path().extension() == ".png")
            {
              texID = (ImTextureID)m_PNGIcon->GetTextureID();
            }
            else if (directoryEntry.path().extension() == ".vert")
            {
              texID = (ImTextureID)m_VertexShaderIcon->GetTextureID();
            }
            else if (directoryEntry.path().extension() == ".tesc")
            {
              texID =
                (ImTextureID)m_TessellationControlShaderIcon->GetTextureID();
            }
            else if (directoryEntry.path().extension() == ".tese")
            {
              texID =
                (ImTextureID)m_TessellationEvaluationShaderIcon->GetTextureID();
            }
            else if (directoryEntry.path().extension() == ".geom")
            {
              texID = (ImTextureID)m_GeometryShaderIcon->GetTextureID();
            }
            else if (directoryEntry.path().extension() == ".frag")
            {
              texID = (ImTextureID)m_FragmentShaderIcon->GetTextureID();
            }
            else if (directoryEntry.path().extension() == ".comp")
            {
              texID = (ImTextureID)m_ComputeShaderIcon->GetTextureID();
            }
            else if (directoryEntry.path().extension() == ".hlsl")
            {
              texID = (ImTextureID)m_HLSLShaderIcon->GetTextureID();
            }
            else if (directoryEntry.path().extension() == ".dscene")
            {
              texID = (ImTextureID)m_SceneIcon->GetTextureID();
            }
            else if (directoryEntry.path().extension() == ".dmat")
            {
              texID = (ImTextureID)m_MaterialIcon->GetTextureID();
            }
            else
            {
              texID = (ImTextureID)m_UnknownFileIcon->GetTextureID();
            }
          }
          else
          {
            texID = (ImTextureID)m_UnknownFileIcon->GetTextureID();
          }
        }
        ImGui::Image(texID, ImVec2(cellWidth - padding, cellWidth - padding));

        ImGui::SetCursorPos(
          ImVec2(cellMin.x + halfPadding, cellMin.y + cellWidth));
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + textWidth);
        ImGui::TextWrapped("%s", relativePath.stem().string().c_str());
        ImGui::PopTextWrapPos();

        if ((ImGui::GetContentRegionAvail().x - cellMin.x - cellWidth) >=
            cellWidth)
        {
          column++;
        }
        else
        {
          rowOffset += tallestCell + halfPadding;
          tallestCell = 0;
          column = 0;
        }
      }
    }

    if (m_OpenRename)
    {
      ImGui::OpenPopup("rename_popup");
      m_OpenRename = false;
    }

    if (ImGui::BeginPopup("rename_popup"))
    {
      std::filesystem::path old = m_RenamePathBuffer;

      ImGui::InputText("##RenameInput", m_RenameBuffer, 128);
      if (ImGui::Button("Rename##RenameButton") &&
          (std::strlen(m_RenameBuffer) > 0))
      {
        std::filesystem::path newPath =
          m_RenamePathBuffer.remove_filename().concat(m_RenameBuffer);
        if (AssetDatabase::Exists(old))
        {
          AssetDatabase::Rename(old, newPath);
        }
        else
        {
          AssetDatabase::RenameDirectory(old, newPath);
        }
        FileHandler::Rename(old, newPath);
        ImGui::CloseCurrentPopup();
      }

      ImGui::SameLine();

      if (ImGui::Button("Cancel"))
      {
        ImGui::CloseCurrentPopup();
      }
      ImGui::EndPopup();
    }

    ImGui::End();
  }

  void
  AssetBrowserWindow::GoBack()
  {
    if (m_HistoryPos > 0)
    {
      m_HistoryPos--;
      m_CurrentDirectory = m_DirectoryHistory[m_HistoryPos];
    }
  }

  void
  AssetBrowserWindow::GoForward()
  {
    if (m_HistoryPos < (m_DirectoryHistory.size() - 1))
    {
      m_HistoryPos++;
      m_CurrentDirectory = m_DirectoryHistory[m_HistoryPos];
    }
  }

  void
  AssetBrowserWindow::OpenPath(
    std::filesystem::directory_entry const& directoryEntry)
  {
    if (directoryEntry.is_directory())
    {
      EnterDirectory(directoryEntry.path().filename());
    }
    else if (directoryEntry.path().extension() == ".dscene")
    {
      m_Model->SetScene(SceneUtilities::LoadScene(directoryEntry.path()));
    }
    else
    {
      // TODO:s Open file
      FileHandler::LaunchFile(directoryEntry.path());
    }
  }

  void
  AssetBrowserWindow::EnterDirectory(std::filesystem::path const& path)
  {
    m_CurrentDirectory /= path.filename();
    if (m_HistoryPos < (m_DirectoryHistory.size() - 1))
    {
      for (int i = 0; i < (m_DirectoryHistory.size() - m_HistoryPos); i++)
      {
        m_DirectoryHistory.pop_back();
      }
    }
    m_DirectoryHistory.push_back(m_CurrentDirectory);
    m_HistoryPos = (int)m_DirectoryHistory.size() - 1;
  }

  void
  AssetBrowserWindow::SelectAsset(std::filesystem::path const& path)
  {
    m_SelectedAsset = path;
    m_Model->GetSelection().SelectAsset(m_SelectedAsset);
    // TODO: command to inspector
  }

  void
  AssetBrowserWindow::HandleShortcuts() const
  {
    // TODO: Implement shortcut handling
  }

  void
  AssetBrowserWindow::ClearSelection()
  {
    m_SelectedAsset = "";
    m_Model->GetSelection().ClearAssetSelection();
  }

  void
  AssetBrowserWindow::Deserialize(nlohmann::json moduleData)
  {
    m_CurrentDirectory =
      (std::filesystem::path)moduleData["openedPath"].get<std::string>();
  }

  std::string
  AssetBrowserWindow::Serialize()
  {
    nlohmann::json moduleData;

    moduleData["openedPath"] = m_CurrentDirectory;

    return moduleData.dump(4);
  }

  void
  AssetBrowserWindow::SetRenameBuffer(std::filesystem::path const& path)
  {
    if (FileHandler::CheckIfFileExists(path))
    {
      std::strncpy(
        m_RenameBuffer,
        std::filesystem::path(m_RenamePathBuffer).filename().string().c_str(),
        RENAME_BUFFER_SIZE);
    }
    else if (FileHandler::CheckIfDirectoyExists(path))
    {
      std::strncpy(
        m_RenameBuffer,
        std::filesystem::path(m_RenamePathBuffer).stem().string().c_str(),
        RENAME_BUFFER_SIZE);
    }
  }
}