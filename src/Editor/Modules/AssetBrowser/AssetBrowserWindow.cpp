#include "pch.h"

#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include "UI/DwarfUI.h"
#include <algorithm>
#include <filesystem>
#include <imgui.h>
#include <imgui_internal.h>

namespace Dwarf
{
  AssetBrowserWindow::AssetBrowserWindow(
    AssetDirectoryPath                            assetDirectoryPath,
    std::shared_ptr<IDwarfLogger>                 logger,
    std::shared_ptr<ITextureFactory>              textureFactory,
    std::shared_ptr<IAssetDatabase>               assetDatabase,
    std::shared_ptr<IInputManager>                inputManager,
    std::shared_ptr<IEditorSelection>             editorSelection,
    std::shared_ptr<IMaterialIO>                  materialIO,
    std::shared_ptr<IMaterialFactory>             materialFactory,
    std::shared_ptr<IAssetMetadata>               assetMetadata,
    std::shared_ptr<IMaterialCreator>             materialCreator,
    std::shared_ptr<IShaderCreator>               shaderCreator,
    std::shared_ptr<IFileHandler>                 fileHandler,
    std::shared_ptr<ISceneIO>                     sceneIO,
    std::shared_ptr<ILoadedScene>                 loadedScene,
    std::shared_ptr<IAssetBrowserListenerFactory> assetBrowserListenerFactory)
    : IGuiModule(ModuleLabel("Asset Browser"),
                 ModuleType(MODULE_TYPE::ASSET_BROWSER),
                 ModuleID(std::make_shared<UUID>()))
    , mAssetDirectoryPath(assetDirectoryPath)
    , mLogger(std::move(logger))
    , mTextureFactory(std::move(textureFactory))
    , mAssetDatabase(std::move(assetDatabase))
    , mInputManager(std::move(inputManager))
    , mEditorSelection(std::move(editorSelection))
    , mMaterialIO(std::move(materialIO))
    , mMaterialFactory(std::move(materialFactory))
    , mAssetMetadata(std::move(assetMetadata))
    , mMaterialCreator(std::move(materialCreator))
    , mShaderCreator(std::move(shaderCreator))
    , mFileHandler(std::move(fileHandler))
    , mSceneIo(std::move(sceneIO))
    , mLoadedScene(std::move(loadedScene))
    , mAssetBrowserListener(assetBrowserListenerFactory->Create(
        [this]()
        {
          InvalidateDirectoryStructureCache();
          InvalidateDirectoryContentCache();
        }))
  {
    mData.CurrentDirectory = mAssetDirectoryPath;
    mData.DirectoryHistory.push_back(mData.CurrentDirectory);
    LoadIcons();
    mLogger->LogDebug(Log("AssetBrowserWindow created", "AssetBrowserWindow"));
  }

  AssetBrowserWindow::AssetBrowserWindow(
    AssetDirectoryPath                            assetDirectoryPath,
    std::shared_ptr<IDwarfLogger>                 logger,
    std::shared_ptr<ITextureFactory>              textureFactory,
    std::shared_ptr<IAssetDatabase>               assetDatabase,
    std::shared_ptr<IInputManager>                inputManager,
    std::shared_ptr<IEditorSelection>             editorSelection,
    std::shared_ptr<IMaterialIO>                  materialIO,
    std::shared_ptr<IMaterialFactory>             materialFactory,
    std::shared_ptr<IAssetMetadata>               assetMetadata,
    std::shared_ptr<IMaterialCreator>             materialCreator,
    std::shared_ptr<IShaderCreator>               shaderCreator,
    std::shared_ptr<IFileHandler>                 fileHandler,
    std::shared_ptr<ISceneIO>                     sceneIO,
    std::shared_ptr<ILoadedScene>                 loadedScene,
    std::shared_ptr<IAssetBrowserListenerFactory> assetBrowserListenerFactory,
    SerializedModule                              serializedModule)
    : IGuiModule(ModuleLabel("Asset Browser"),
                 ModuleType(MODULE_TYPE::ASSET_BROWSER),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , mAssetDirectoryPath(assetDirectoryPath)
    , mLogger(std::move(logger))
    , mTextureFactory(std::move(textureFactory))
    , mAssetDatabase(std::move(assetDatabase))
    , mInputManager(std::move(inputManager))
    , mEditorSelection(std::move(editorSelection))
    , mMaterialIO(std::move(materialIO))
    , mMaterialFactory(std::move(materialFactory))
    , mAssetMetadata(std::move(assetMetadata))
    , mMaterialCreator(std::move(materialCreator))
    , mShaderCreator(std::move(shaderCreator))
    , mFileHandler(std::move(fileHandler))
    , mSceneIo(std::move(sceneIO))
    , mLoadedScene(std::move(loadedScene))
    , mAssetBrowserListener(assetBrowserListenerFactory->Create(
        [this]()
        {
          InvalidateDirectoryStructureCache();
          InvalidateDirectoryContentCache();
        }))
  {
    mData.CurrentDirectory = mAssetDirectoryPath;
    mData.DirectoryHistory.push_back(mData.CurrentDirectory);
    LoadIcons();
    // Deserialize(serializedModule.t);
    mLogger->LogDebug(Log("AssetBrowserWindow created", "AssetBrowserWindow"));
  }

  AssetBrowserWindow::~AssetBrowserWindow()
  {
    mLogger->LogDebug(
      Log("AssetBrowserWindow destroyed", "AssetBrowserWindow"));
  }

  void
  AssetBrowserWindow::SetupDockspace(ImGuiID imguiId)
  {
    ImGui::DockBuilderRemoveNode(
      imguiId); // Clear any preexisting layouts associated with the ID we just
                // chose
    ImGui::DockBuilderAddNode(imguiId, ImGuiDockNodeFlags_HiddenTabBar);

    footerID = ImGui::DockBuilderSplitNode(
      imguiId, ImGuiDir_Down, 0.07F, nullptr, &imguiId);
    ImGuiID folderStructureID = ImGui::DockBuilderSplitNode(
      imguiId, ImGuiDir_Left, 0.3F, nullptr, &imguiId);
    ImGuiID folderContent = ImGui::DockBuilderSplitNode(
      folderStructureID, ImGuiDir_Right, 0.7F, nullptr, &folderStructureID);

    // 6. Add windows to each docking space:
    ImGui::DockBuilderDockWindow("FolderStructure", folderStructureID);
    ImGui::DockBuilderDockWindow("Footer", footerID);
    ImGui::DockBuilderDockWindow("FolderContent", folderContent);

    // 7. We're done setting up our docking configuration:
    ImGui::DockBuilderFinish(imguiId);
  }

  void
  AssetBrowserWindow::OnUpdate()
  {
    if (!mWindowOpened)
    {
      return;
    }

    if (mInputManager->GetMouseButtonDown(MOUSE_BUTTON::MOUSE_BUTTON_4))
    {
      GoBack();
    }
    else if (mInputManager->GetMouseButtonDown(MOUSE_BUTTON::MOUSE_BUTTON_5))
    {
      GoForward();
    }

    {
      std::unique_lock<std::mutex> lock(mDirectoryStructureCache.ValidityMutex);

      if (!mDirectoryStructureCache.Valid)
      {
        RebuildDirectoryStructureCache();
      }
    }
    {
      std::unique_lock<std::mutex> lock(mDirectoryContentCache.ValidityMutex);
      if (!mDirectoryContentCache.Valid)
      {
        RebuildDirectoryContentCache(mData.CurrentDirectory);
      }
    }
  }

  void
  AssetBrowserWindow::LoadIcons()
  {
    std::filesystem::path iconPath("data/engine/img/icons/asset browser");

    mDirectoryIcon = mTextureFactory->FromPath(iconPath / "directoryIcon.png");

    mFBXIcon = mTextureFactory->FromPath(iconPath / "fbxIcon.png");
    mOBJIcon = mTextureFactory->FromPath(iconPath / "objIcon.png");

    mJPGIcon = mTextureFactory->FromPath(iconPath / "jpgIcon.png");
    mPNGIcon = mTextureFactory->FromPath(iconPath / "pngIcon.png");

    mVertexShaderIcon =
      mTextureFactory->FromPath(iconPath / "vertexShaderIcon.png");
    mTessellationControlShaderIcon =
      mTextureFactory->FromPath(iconPath / "tessellationControlShaderIcon.png");
    mTessellationEvaluationShaderIcon = mTextureFactory->FromPath(
      iconPath / "tessellationEvaluationShaderIcon.png");
    mGeometryShaderIcon =
      mTextureFactory->FromPath(iconPath / "geometryShaderIcon.png");
    mFragmentShaderIcon =
      mTextureFactory->FromPath(iconPath / "fragmentShaderIcon.png");
    mComputeShaderIcon =
      mTextureFactory->FromPath(iconPath / "computeShaderIcon.png");
    mHLSLShaderIcon =
      mTextureFactory->FromPath(iconPath / "hlslShaderIcon.png");

    mSceneIcon = mTextureFactory->FromPath(iconPath / "sceneIcon.png");

    mMaterialIcon = mTextureFactory->FromPath(iconPath / "materialIcon.png");

    mUnknownFileIcon =
      mTextureFactory->FromPath(iconPath / "unknownFileIcon.png");
  }

  auto
  CompareDirectoryEntries(std::filesystem::directory_entry const& d1,
                          std::filesystem::directory_entry const& d2) -> bool
  {
    return d1.path().stem() < d2.path().stem();
  }

  void
  AssetBrowserWindow::OnImGuiRender()
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500, 500));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    if (!ImGui::Begin(GetIdentifier().c_str(), &mWindowOpened, 0))
    {
      // Early out if the window is collapsed, as an optimization.
      ImGui::End();
      ImGui::PopStyleVar(2);
      return;
    }
    ImGui::PopStyleVar(2);

    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
    dockspaceFlags |= ImGuiDockNodeFlags_HiddenTabBar;
    ImGuiID dockspaceId = ImGui::GetID("AssetBrowserDockspace");
    ImGui::DockSpace(dockspaceId, ImVec2(0.0F, 0.0F), dockspaceFlags);

    static bool firstFrame = true;
    if (firstFrame)
    {
      firstFrame = false;
      SetupDockspace(dockspaceId);
    }

    RenderDirectoryStructure();
    RenderDirectoryContent();
    RenderFooter();

    ImGui::End();
  }

  auto
  to_lower(const std::string& str) -> std::string
  {
    std::string result = str;
    std::ranges::transform(
      result, result.begin(), [](unsigned char c) { return std::tolower(c); });
    return result;
  }

  void
  AssetBrowserWindow::RebuildDirectoryContentCache(
    const std::filesystem::path& directory)
  {
    mDirectoryContentCache.Entries.clear();

    for (const auto& directoryEntry :
         std::filesystem::directory_iterator(directory))
    {
      if ((directoryEntry.is_directory() ||
           (directoryEntry.is_regular_file() &&
            (directoryEntry.path().has_extension() &&
             directoryEntry.path().extension() !=
               IAssetMetadata::METADATA_EXTENSION))) &&
          (!mData.SearchBuffer.empty()
             ? (to_lower(directoryEntry.path().filename().string())
                  .find(to_lower(mData.SearchBuffer)) != std::string::npos)
             : true))
      {
        DirectoryEntryData data;
        data.Path = directoryEntry.path();
        data.Thumbnail = GetTextureIdForDirectoryEntry(directoryEntry);
        data.IsDirectory = directoryEntry.is_directory();
        mDirectoryContentCache.Entries.emplace_back(data);
      }
    }

    // Sort alphabetically and have directories first
    std::ranges::sort(
      mDirectoryContentCache.Entries,
      [](const DirectoryEntryData& a, const DirectoryEntryData& b)
      {
        bool aIsDir = a.IsDirectory;
        bool bIsDir = b.IsDirectory;

        // First, sort directories before files
        if (aIsDir != bIsDir)
        {
          return aIsDir > bIsDir; // Directories first
        }

        // Then, sort alphabetically
        return a.Path.filename().string() < b.Path.filename().string();
      });

    mDirectoryContentCache.Valid = true;
  }

  auto
  AssetBrowserWindow::GenerateDirectoryDataRecursively(
    const std::filesystem::path& directory) -> DirectoryData
  {
    DirectoryData data;
    data.Path = directory;
    for (const auto& directoryEntry :
         std::filesystem::directory_iterator(directory))
    {
      if (directoryEntry.is_directory())
      {
        data.Subdirectories.emplace_back(
          GenerateDirectoryDataRecursively(directoryEntry.path()));
      }
    }

    // Sort directories
    std::ranges::stable_sort(data.Subdirectories,
                             [](const DirectoryData& a, const DirectoryData& b)
                             { return a.Path.stem() < b.Path.stem(); });

    return data;
  }

  void
  AssetBrowserWindow::RebuildDirectoryStructureCache()
  {
    mDirectoryStructureCache.RootDirectoryData =
      GenerateDirectoryDataRecursively(mAssetDirectoryPath);
    mDirectoryStructureCache.Valid = true;
  }

  void
  AssetBrowserWindow::RenderDirectoryStructureCacheRecursively(
    const DirectoryData& data)
  {
    ImGuiTreeNodeFlags flags =
      ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    std::string label = data.Path.stem().string();
    ImGuiID     id = ImGui::GetID(data.Path.string().c_str());

    // Check if this directory is the current directory
    if (data.Path == mData.CurrentDirectory)
    {
      flags |= ImGuiTreeNodeFlags_Selected;
    }

    // Check if this directory has any subdirectories
    if (data.Subdirectories.empty())
    {
      flags |= ImGuiTreeNodeFlags_Leaf;
    }

    bool currentlyOpen =
      ImGui::TreeNodeEx((void*)(intptr_t)id, flags, "%s", label.c_str());

    if (ImGui::IsItemHovered())
    {
      ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
      EnterDirectory(data.Path);
    }

    if (currentlyOpen)
    {
      for (const auto& subDirectory : data.Subdirectories)
      {
        RenderDirectoryStructureCacheRecursively(subDirectory);
      }
      ImGui::TreePop();
    }
  }

  void
  AssetBrowserWindow::RenderDirectoryStructure()
  {
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_NoMove;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(150, 150));
    ImGui::Begin("FolderStructure", nullptr, windowFlags);
    RenderDirectoryStructureCacheRecursively(
      mDirectoryStructureCache.RootDirectoryData);
    ImGui::End();
    ImGui::PopStyleVar();
  }

  void
  AssetBrowserWindow::RenderFooter()
  {
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoResize;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500, 500));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0F, 5.0F));
    ImGui::Begin("Footer", nullptr, windowFlags);
    if (ImGui::Button("<", ImVec2(20, 20)))
    {
      GoBack();
    }
    ImGui::SameLine(0.0F, 5.0F);
    if (ImGui::Button(">", ImVec2(20, 20)))
    {
      GoForward();
    }
    ImGui::SameLine(0.0F, 5.0F);
    ImGui::PushItemWidth(100);
    ImGui::SliderFloat("Size", &mData.IconScale, 1.0F, 2.0F);
    ImGui::SameLine(0.0F, 5.0F);
    if (ImGui::InputText("Search##SearchInput",
                         mData.SearchBuffer.data(),
                         mData.SearchBuffer.capacity() + 1,
                         ImGuiInputTextFlags_CallbackResize,
                         DwarfUI::InputTextCallback,
                         &mData.SearchBuffer))
    {
      mDirectoryContentCache.Valid = false;
    }
    ImGui::PopItemWidth();
    ImGui::End();
    ImGui::PopStyleVar(2);
  }

  void
  AssetBrowserWindow::RenderDirectoryContent()
  {
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_NoMove;

    ImGui::Begin("FolderContent", &mWindowOpened, windowFlags);

    // Rendering the entries
    {
      float tallestCell = 0;
      int   colCount = 0;

      for (auto const& directoryEntry : mDirectoryContentCache.Entries)
      {
        float padding = 16.0F * mData.IconScale;
        float halfPadding = padding / 2.0F;
        float cellWidth = 64.0F * mData.IconScale;
        float textWidth = cellWidth - padding;
        float textHeight =
          ImGui::CalcTextSize(directoryEntry.Path.stem().string().c_str(),
                              nullptr,
                              false,
                              textWidth)
            .y;
        float cellHeight = cellWidth + textHeight + halfPadding;

        tallestCell = std::max(cellHeight, tallestCell);

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.0F, 1.0F, 1.0F, 0.2F));
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered,
                              ImVec4(1.0F, 1.0F, 1.0F, 0.2F));
        ImGui::PushStyleColor(ImGuiCol_HeaderActive,
                              ImVec4(1.0F, 1.0F, 1.0F, 0.4F));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

        ImVec2 startPos = ImGui::GetCursorPos();
        ImVec2 imagePos = { startPos.x + halfPadding,
                            startPos.y + halfPadding };
        ImVec2 textPos = { imagePos.x, imagePos.y + cellWidth - halfPadding };
        std::string id = std::string("##").append(directoryEntry.Path.string());
        ImGui::SetCursorPos(imagePos);
        ImGui::Image(directoryEntry.Thumbnail,
                     ImVec2(cellWidth - padding, cellWidth - padding));

        ImGui::SetCursorPos(textPos);
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + textWidth);
        ImGui::TextWrapped("%s", directoryEntry.Path.stem().string().c_str());
        ImGui::PopTextWrapPos();

        ImGui::SetCursorPos(startPos);
        ImGui::Selectable(
          id.c_str(),
          mEditorSelection->IsAssetSelected(directoryEntry.Path),
          ImGuiSelectableFlags_AllowDoubleClick,
          ImVec2(cellWidth, cellHeight));

        if (ImGui::IsItemHovered())
        {
          ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) ||
            ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
          if (!directoryEntry.IsDirectory &&
              mAssetDatabase->Exists(directoryEntry.Path))
          {
            mEditorSelection->SelectAsset(
              mAssetDatabase->Retrieve(directoryEntry.Path));
          }

          if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
          {
            if (directoryEntry.IsDirectory)
            {
              EnterDirectory(directoryEntry.Path);
            }
            else
            {
              OpenPath(directoryEntry.Path);
            }
          }
        }

        if (ImGui::BeginPopupContextItem(
              directoryEntry.Path.string().c_str())) // <-- use last item id
                                                     // as popup id
        {
          if (ImGui::MenuItem("Open"))
          {
            if (directoryEntry.IsDirectory)
            {
              EnterDirectory(directoryEntry.Path);
            }
            else
            {
              OpenPath(directoryEntry.Path);
            }
          }
          else if (ImGui::MenuItem("Copy"))
          {
            mData.CopyPathBuffer = directoryEntry.Path;
          }
          else if (ImGui::MenuItem("Duplicate"))
          {
            mFileHandler->Duplicate(directoryEntry.Path);
          }
          else if (ImGui::MenuItem("Rename"))
          {
            mData.RenamePathBuffer = directoryEntry.Path;
            SetRenameBuffer(directoryEntry.Path);
            mData.OpenRename = true;
          }
          if (ImGui::MenuItem("Open in file browser"))
          {
            if (directoryEntry.IsDirectory)
            {
              mFileHandler->OpenPathInFileBrowser(directoryEntry.Path);
            }
            else
            {
              mFileHandler->OpenPathInFileBrowser(mData.CurrentDirectory);
            }
          }
          else if (ImGui::MenuItem("Delete"))
          {
            // TODO: Confirmation modal
            if (mAssetDatabase->Exists(directoryEntry.Path))
            {
              mAssetDatabase->Remove(directoryEntry.Path);
              mAssetMetadata->RemoveMetadata(directoryEntry.Path);
            }
            mFileHandler->Delete(directoryEntry.Path);
          }
          ImGui::EndPopup();
        }

        if ((ImGui::GetCursorPosX() + ((colCount + 1) * (cellWidth + 8.0F))) <
            (ImGui::GetContentRegionAvail().x - (cellWidth - 8.0F)))
        {
          ImGui::SameLine(0, 8.0F);
          colCount++;
        }
        else
        {
          colCount = 0;
          ImGui::SetCursorPosY(ImGui::GetCursorPosY() +
                               (tallestCell - cellHeight) + 8.0F);
          tallestCell = 0;
        }
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar(1);
      }
    }

    // Rename popup
    {
      if (mData.OpenRename)
      {
        ImGui::OpenPopup("rename_popup");
        mData.OpenRename = false;
      }

      if (ImGui::BeginPopup("rename_popup"))
      {
        std::filesystem::path old = mData.RenamePathBuffer;

        ImGui::InputText("##RenameInput",
                         mData.RenameBuffer.data(),
                         mData.RenameBuffer.capacity() + 1,
                         ImGuiInputTextFlags_CallbackResize,
                         DwarfUI::InputTextCallback,
                         &mData.RenameBuffer);
        if (ImGui::Button("Rename##RenameButton") &&
            (mData.RenameBuffer.size() > 0))
        {
          std::filesystem::path newPath =
            mData.RenamePathBuffer.remove_filename().concat(mData.RenameBuffer);
          mFileHandler->Rename(old, newPath);
          InvalidateDirectoryStructureCache();
          InvalidateDirectoryContentCache();
          ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
          ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
      }
    }

    // Right Click menu
    {
      if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() &&
          ImGui::IsMouseClicked(ImGuiMouseButton_Right))
      {
        ImGui::OpenPopup("ContextMenu");
      }

      if (ImGui::BeginPopup("ContextMenu")) // <-- use last item id as popup id
      {
        if (ImGui::BeginMenu("New"))
        {
          if (ImGui::BeginMenu("Material"))
          {
            if (ImGui::MenuItem("Pbr"))
            {
              mMaterialCreator->CreateMaterialAsset(MaterialType::PbrMaterial,
                                                    mData.CurrentDirectory);
            }
            if (ImGui::MenuItem("Unlit"))
            {
              mMaterialCreator->CreateMaterialAsset(MaterialType::UnlitMaterial,
                                                    mData.CurrentDirectory);
            }
            ImGui::EndMenu();
          }

          if (ImGui::BeginMenu("Shader"))
          {
            if (ImGui::BeginMenu("Vertex"))
            {
              if (ImGui::MenuItem("Pbr"))
              {
                mShaderCreator->CreateShaderAsset(ShaderType::VertexType,
                                                  ShaderSource::PbrSource,
                                                  mData.CurrentDirectory);
              }

              if (ImGui::MenuItem("Unlit"))
              {
                mShaderCreator->CreateShaderAsset(ShaderType::VertexType,
                                                  ShaderSource::UnlitSource,
                                                  mData.CurrentDirectory);
              }
              ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Fragment"))
            {
              if (ImGui::MenuItem("Pbr"))
              {
                mShaderCreator->CreateShaderAsset(ShaderType::FragmentType,
                                                  ShaderSource::PbrSource,
                                                  mData.CurrentDirectory);
              }

              if (ImGui::MenuItem("Unlit"))
              {
                mShaderCreator->CreateShaderAsset(ShaderType::FragmentType,
                                                  ShaderSource::UnlitSource,
                                                  mData.CurrentDirectory);
              }
              ImGui::EndMenu();
            }

            ImGui::EndMenu();
          }

          if (ImGui::MenuItem("Scene"))
          {
            mSceneIo->NewSceneAsset(mData.CurrentDirectory);
          }

          ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Create Folder"))
        {
          mFileHandler->CreateDirectoryAt(mData.CurrentDirectory /
                                          "New Folder");
        }

        if (ImGui::MenuItem("Paste"))
        {
          if (std::filesystem::exists(mData.CopyPathBuffer))
          {
            mFileHandler->Copy(mData.CopyPathBuffer, mData.CurrentDirectory);
          }
        }

        if (ImGui::MenuItem("Open in file browser"))
        {
          mFileHandler->OpenPathInFileBrowser(mData.CurrentDirectory);
        }
        ImGui::EndPopup();
      }
    }

    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
      mEditorSelection->ClearAssetSelection();
    }

    ImGui::End();
  }

  void
  AssetBrowserWindow::GoBack()
  {
    if (mData.HistoryPos > 0)
    {
      mData.HistoryPos--;
      if (mData.CurrentDirectory != mData.DirectoryHistory[mData.HistoryPos])
      {
        mDirectoryContentCache.Valid = false;
      }
      mData.CurrentDirectory = mData.DirectoryHistory[mData.HistoryPos];
    }
  }

  void
  AssetBrowserWindow::GoForward()
  {
    if (mData.HistoryPos < (mData.DirectoryHistory.size() - 1))
    {
      mData.HistoryPos++;
      mData.CurrentDirectory = mData.DirectoryHistory[mData.HistoryPos];
    }
  }

  void
  AssetBrowserWindow::OpenPath(const std::filesystem::path& path)
  {
    if (path.extension() == ".dscene")
    {
      if (mAssetDatabase->Exists(path))
      {
        auto sceneRef = mAssetDatabase->Retrieve(path);
        auto loadedScene = mSceneIo->LoadScene(*sceneRef);
        mLoadedScene->SetScene(std::move(loadedScene));
      }
    }
    else
    {
      mFileHandler->LaunchFile(path);
    }
  }

  void
  AssetBrowserWindow::UnfoldSelectedDirectory()
  {
  }

  void
  AssetBrowserWindow::EnterDirectory(std::filesystem::path const& path)
  {
    if (mData.CurrentDirectory == path)
    {
      return;
    }

    mData.CurrentDirectory = path;
    mDirectoryContentCache.Valid = false;

    if (mData.HistoryPos < (mData.DirectoryHistory.size() - 1))
    {
      for (int i = 0; i < (mData.DirectoryHistory.size() - mData.HistoryPos);
           i++)
      {
        mData.DirectoryHistory.pop_back();
      }
    }
    mData.DirectoryHistory.push_back(mData.CurrentDirectory);
    mData.HistoryPos = (int)mData.DirectoryHistory.size() - 1;
  }

  void
  AssetBrowserWindow::HandleShortcuts() const
  {
    // TODO: Implement shortcut handling
  }

  void
  AssetBrowserWindow::ClearSelection()
  {
    mEditorSelection->ClearAssetSelection();
  }

  void
  AssetBrowserWindow::Deserialize(nlohmann::json moduleData)
  {
    mData.CurrentDirectory =
      (std::filesystem::path)moduleData["openedPath"].get<std::string>();
  }

  auto
  AssetBrowserWindow::Serialize() -> nlohmann::json
  {
    nlohmann::json serializedModule;

    serializedModule["openedPath"] = mData.CurrentDirectory;
    serializedModule["id"] = GetUuid()->toString();
    serializedModule["type"] = static_cast<int>(GetModuleType());
    serializedModule["label"] = GetModuleName();

    return serializedModule;
  }

  void
  AssetBrowserWindow::SetRenameBuffer(std::filesystem::path const& path)
  {
    mData.RenameBuffer = mData.RenamePathBuffer.filename().string();
  }

  auto
  AssetBrowserWindow::GetTextureIdForDirectoryEntry(
    const std::filesystem::directory_entry& directoryEntry) -> ImTextureID
  {
    auto texID = (ImTextureID)mUnknownFileIcon->GetTextureID();

    if (directoryEntry.is_directory())
    {
      texID = (ImTextureID)mDirectoryIcon->GetTextureID();
    }
    else if (directoryEntry.is_regular_file())
    {
      if (directoryEntry.path().has_extension())
      {
        std::string extension = boost::algorithm::to_lower_copy(
          directoryEntry.path().extension().string());
        if (extension == ".fbx")
        {
          texID = (ImTextureID)mFBXIcon->GetTextureID();
        }
        else if (extension == ".obj")
        {
          texID = (ImTextureID)mOBJIcon->GetTextureID();
        }
        else if (extension == ".jpg")
        {
          texID = (ImTextureID)mJPGIcon->GetTextureID();
        }
        else if (extension == ".png")
        {
          texID = (ImTextureID)mPNGIcon->GetTextureID();
        }
        else if (extension == ".vert")
        {
          texID = (ImTextureID)mVertexShaderIcon->GetTextureID();
        }
        else if (extension == ".tesc")
        {
          texID = (ImTextureID)mTessellationControlShaderIcon->GetTextureID();
        }
        else if (extension == ".tese")
        {
          texID =
            (ImTextureID)mTessellationEvaluationShaderIcon->GetTextureID();
        }
        else if (extension == ".geom")
        {
          texID = (ImTextureID)mGeometryShaderIcon->GetTextureID();
        }
        else if (extension == ".frag")
        {
          texID = (ImTextureID)mFragmentShaderIcon->GetTextureID();
        }
        else if (extension == ".comp")
        {
          texID = (ImTextureID)mComputeShaderIcon->GetTextureID();
        }
        else if (extension == ".hlsl")
        {
          texID = (ImTextureID)mHLSLShaderIcon->GetTextureID();
        }
        else if (extension == ".dscene")
        {
          texID = (ImTextureID)mSceneIcon->GetTextureID();
        }
        else if (extension == ".dmat")
        {
          texID = (ImTextureID)mMaterialIcon->GetTextureID();
        }
      }
    }

    return texID;
  }

  void
  AssetBrowserWindow::InvalidateDirectoryStructureCache()
  {
    std::unique_lock<std::mutex> lock(mDirectoryStructureCache.ValidityMutex);
    mDirectoryStructureCache.Valid = false;
  }

  void
  AssetBrowserWindow::InvalidateDirectoryContentCache()
  {
    std::unique_lock<std::mutex> lock(mDirectoryContentCache.ValidityMutex);
    mDirectoryContentCache.Valid = false;
  }
}