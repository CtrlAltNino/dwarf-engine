#include "pch.h"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
// #include "Core/Scene/SceneUtilities.h"
#include <cstring>
#include <memory>
#include <boost/algorithm/string.hpp>

namespace Dwarf
{
  AssetBrowserWindow::AssetBrowserWindow(
    AssetDirectoryPath                assetDirectoryPath,
    std::shared_ptr<ITextureFactory>  textureFactory,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IMaterialIO>      materialIO,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IAssetMetadata>   assetMetadata,
    std::shared_ptr<IMaterialCreator> materialCreator,
    std::shared_ptr<IFileHandler>     fileHandler)
    : IGuiModule(ModuleLabel("Asset Browser"),
                 ModuleType(MODULE_TYPE::ASSET_BROWSER),
                 ModuleID(std::make_shared<UUID>()))
    , m_AssetDirectoryPath(assetDirectoryPath)
    , m_CurrentDirectory(assetDirectoryPath)
    , m_TextureFactory(textureFactory)
    , m_AssetDatabase(assetDatabase)
    , m_InputManager(inputManager)
    , m_EditorSelection(editorSelection)
    , m_MaterialIO(materialIO)
    , m_MaterialFactory(materialFactory)
    , m_AssetMetadata(assetMetadata)
    , m_MaterialCreator(materialCreator)
    , m_FileHandler(fileHandler)
  {
    m_DirectoryHistory.push_back(m_CurrentDirectory);
    LoadIcons();
  }

  AssetBrowserWindow::AssetBrowserWindow(
    AssetDirectoryPath                assetDirectoryPath,
    std::shared_ptr<ITextureFactory>  textureFactory,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IMaterialIO>      materialIO,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IAssetMetadata>   assetMetadata,
    std::shared_ptr<IMaterialCreator> materialCreator,
    std::shared_ptr<IFileHandler>     fileHandler,
    SerializedModule                  serializedModule)
    : IGuiModule(ModuleLabel("Asset Browser"),
                 ModuleType(MODULE_TYPE::ASSET_BROWSER),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , m_AssetDirectoryPath(assetDirectoryPath)
    , m_CurrentDirectory(assetDirectoryPath)
    , m_TextureFactory(textureFactory)
    , m_AssetDatabase(assetDatabase)
    , m_InputManager(inputManager)
    , m_EditorSelection(editorSelection)
    , m_MaterialIO(materialIO)
    , m_MaterialFactory(materialFactory)
    , m_AssetMetadata(assetMetadata)
    , m_MaterialCreator(materialCreator)
    , m_FileHandler(fileHandler)
  {
    m_DirectoryHistory.push_back(m_CurrentDirectory);
    LoadIcons();
    // Deserialize(serializedModule.t);
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
  AssetBrowserWindow::OnUpdate()
  {
    if (m_InputManager->GetMouseButtonDown(MOUSE_BUTTON::MOUSE_BUTTON_4))
    {
      GoBack();
    }
    else if (m_InputManager->GetMouseButtonDown(MOUSE_BUTTON::MOUSE_BUTTON_5))
    {
      GoForward();
    }
  }

  void
  AssetBrowserWindow::LoadIcons()
  {
    std::filesystem::path iconPath("data/engine/img/icons/asset browser");

    m_DirectoryIcon =
      m_TextureFactory->FromPath(iconPath / "directoryIcon.png");

    m_FBXIcon = m_TextureFactory->FromPath(iconPath / "fbxIcon.png");
    m_OBJIcon = m_TextureFactory->FromPath(iconPath / "objIcon.png");

    m_JPGIcon = m_TextureFactory->FromPath(iconPath / "jpgIcon.png");
    m_PNGIcon = m_TextureFactory->FromPath(iconPath / "pngIcon.png");

    m_VertexShaderIcon =
      m_TextureFactory->FromPath(iconPath / "vertexShaderIcon.png");
    m_TessellationControlShaderIcon = m_TextureFactory->FromPath(
      iconPath / "tessellationControlShaderIcon.png");
    m_TessellationEvaluationShaderIcon = m_TextureFactory->FromPath(
      iconPath / "tessellationEvaluationShaderIcon.png");
    m_GeometryShaderIcon =
      m_TextureFactory->FromPath(iconPath / "geometryShaderIcon.png");
    m_FragmentShaderIcon =
      m_TextureFactory->FromPath(iconPath / "fragmentShaderIcon.png");
    m_ComputeShaderIcon =
      m_TextureFactory->FromPath(iconPath / "computeShaderIcon.png");
    m_HLSLShaderIcon =
      m_TextureFactory->FromPath(iconPath / "hlslShaderIcon.png");

    m_SceneIcon = m_TextureFactory->FromPath(iconPath / "sceneIcon.png");

    m_MaterialIcon = m_TextureFactory->FromPath(iconPath / "materialIcon.png");

    m_UnknownFileIcon =
      m_TextureFactory->FromPath(iconPath / "unknownFileIcon.png");
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
        m_CurrentDirectory = m_AssetDirectoryPath.t;
      }
      ImGui::Indent(8.0f);
      RenderDirectoryLevel(m_AssetDirectoryPath.t);
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
            m_MaterialCreator->CreateMaterialAsset(m_CurrentDirectory);
          }
          ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Shader"))
        {
          if (ImGui::MenuItem("Vertex"))
          {
            // TODO: Reimplement this
            // FileHandler::Copy(Shader::GetDefaultShaderPath() /
            // "default.vert",
            //                   m_CurrentDirectory / "New vertex shader.vert");
          }

          if (ImGui::MenuItem("Fragment"))
          {
            // TODO: Reimplement this
            // FileHandler::Copy(Shader::GetDefaultShaderPath() /
            // "default.frag",
            //                   m_CurrentDirectory / "New fragment
            //                   shader.frag");
          }

          ImGui::EndMenu();
        }

        ImGui::MenuItem("Scene");
        ImGui::EndMenu();
      }

      if (ImGui::MenuItem("Create Folder"))
      {
        m_FileHandler->CreateDirectoryAt(m_CurrentDirectory / "New Folder");
      }

      if (ImGui::MenuItem("Paste"))
      {
        if (std::filesystem::exists(m_CopyPathBuffer))
        {
          m_FileHandler->Copy(m_CopyPathBuffer, m_CurrentDirectory);
        }
      }

      if (ImGui::MenuItem("Open in file browser"))
      {
        m_FileHandler->OpenPathInFileBrowser(m_CurrentDirectory);
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
      auto        relativePath =
        std::filesystem::relative(path, m_AssetDirectoryPath.t);

      if (!(directoryEntry.path().has_extension() &&
            directoryEntry.path().extension() ==
              IAssetMetadata::METADATA_EXTENSION))
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
            if (!directoryEntry.is_directory() && m_AssetDatabase->Exists(path))
            {
              m_EditorSelection->SelectAsset(m_AssetDatabase->Retrieve(path));
            }
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
            m_FileHandler->Duplicate(path);
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
              m_FileHandler->OpenPathInFileBrowser(directoryEntry.path());
            }
            else
            {
              m_FileHandler->OpenPathInFileBrowser(m_CurrentDirectory);
            }
          }
          else if (ImGui::MenuItem("Delete"))
          {
            // TODO: Confirmation modal
            if (m_AssetDatabase->Exists(path))
            {
              m_AssetDatabase->Remove(path);
              m_AssetMetadata->RemoveMetadata(path);
            }
            m_FileHandler->Delete(path);
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
            std::string extension = boost::algorithm::to_lower_copy(
              directoryEntry.path().extension().string());
            if (extension == ".fbx")
            {
              texID = (ImTextureID)m_FBXIcon->GetTextureID();
            }
            else if (extension == ".obj")
            {
              texID = (ImTextureID)m_OBJIcon->GetTextureID();
            }
            else if (extension == ".jpg")
            {
              texID = (ImTextureID)m_JPGIcon->GetTextureID();
            }
            else if (extension == ".png")
            {
              texID = (ImTextureID)m_PNGIcon->GetTextureID();
            }
            else if (extension == ".vert")
            {
              texID = (ImTextureID)m_VertexShaderIcon->GetTextureID();
            }
            else if (extension == ".tesc")
            {
              texID =
                (ImTextureID)m_TessellationControlShaderIcon->GetTextureID();
            }
            else if (extension == ".tese")
            {
              texID =
                (ImTextureID)m_TessellationEvaluationShaderIcon->GetTextureID();
            }
            else if (extension == ".geom")
            {
              texID = (ImTextureID)m_GeometryShaderIcon->GetTextureID();
            }
            else if (extension == ".frag")
            {
              texID = (ImTextureID)m_FragmentShaderIcon->GetTextureID();
            }
            else if (extension == ".comp")
            {
              texID = (ImTextureID)m_ComputeShaderIcon->GetTextureID();
            }
            else if (extension == ".hlsl")
            {
              texID = (ImTextureID)m_HLSLShaderIcon->GetTextureID();
            }
            else if (extension == ".dscene")
            {
              texID = (ImTextureID)m_SceneIcon->GetTextureID();
            }
            else if (extension == ".dmat")
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
        if (m_AssetDatabase->Exists(old))
        {
          m_AssetDatabase->Rename(old, newPath);
        }
        else
        {
          m_AssetDatabase->RenameDirectory(old, newPath);
        }
        m_FileHandler->Rename(old, newPath);
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
      // m_Model->SetScene(SceneUtilities::LoadScene(directoryEntry.path()));
      // TODO: Use SceneIO
    }
    else
    {
      // TODO:s Open file
      m_FileHandler->LaunchFile(directoryEntry.path());
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
  AssetBrowserWindow::HandleShortcuts() const
  {
    // TODO: Implement shortcut handling
  }

  void
  AssetBrowserWindow::ClearSelection()
  {
    m_SelectedAsset = "";
    m_EditorSelection->ClearAssetSelection();
  }

  void
  AssetBrowserWindow::Deserialize(nlohmann::json moduleData)
  {
    m_CurrentDirectory =
      (std::filesystem::path)moduleData["openedPath"].get<std::string>();
  }

  nlohmann::json
  AssetBrowserWindow::Serialize()
  {
    nlohmann::json serializedModule;

    serializedModule["openedPath"] = m_CurrentDirectory;
    serializedModule["id"] = GetUuid()->ToString();
    serializedModule["type"] = static_cast<int>(GetModuleType());
    serializedModule["label"] = GetModuleName();

    return serializedModule;
  }

  void
  AssetBrowserWindow::SetRenameBuffer(std::filesystem::path const& path)
  {
    if (m_FileHandler->FileExists(path))
    {
      // TODO: test this
#ifdef _MSC_VER
      strncpy_s(
        m_RenameBuffer,
        std::filesystem::path(m_RenamePathBuffer).filename().string().c_str(),
        RENAME_BUFFER_SIZE - 1);
#else
      strncpy(
        m_RenameBuffer,
        std::filesystem::path(m_RenamePathBuffer).filename().string().c_str(),
        RENAME_BUFFER_SIZE - 1);
#endif
    }
    else if (m_FileHandler->DirectoryExists(path))
    {
      // TODO: test this
#ifdef _MSC_VER
      strncpy_s(
        m_RenameBuffer,
        std::filesystem::path(m_RenamePathBuffer).stem().string().c_str(),
        RENAME_BUFFER_SIZE - 1);
#else
      strncpy(m_RenameBuffer,
              std::filesystem::path(m_RenamePathBuffer).stem().string().c_str(),
              RENAME_BUFFER_SIZE - 1);
#endif
    }
  }
}