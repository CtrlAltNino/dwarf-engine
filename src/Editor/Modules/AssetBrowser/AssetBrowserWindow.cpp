#include "pch.h"

#include "Core/Asset/Creation/Material/IMaterialCreator.h"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include "UI/DwarfUI.h"

namespace Dwarf
{
  AssetBrowserWindow::AssetBrowserWindow(
    AssetDirectoryPath                assetDirectoryPath,
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<ITextureFactory>  textureFactory,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IMaterialIO>      materialIO,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IAssetMetadata>   assetMetadata,
    std::shared_ptr<IMaterialCreator> materialCreator,
    std::shared_ptr<IShaderCreator>   shaderCreator,
    std::shared_ptr<IFileHandler>     fileHandler,
    std::shared_ptr<ISceneIO>         sceneIO)
    : IGuiModule(ModuleLabel("Asset Browser"),
                 ModuleType(MODULE_TYPE::ASSET_BROWSER),
                 ModuleID(std::make_shared<UUID>()))
    , mAssetDirectoryPath(assetDirectoryPath)
    , mLogger(std::move(logger))
    , mCurrentDirectory(assetDirectoryPath)
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
  {
    mDirectoryHistory.push_back(mCurrentDirectory);
    LoadIcons();
    mLogger->LogDebug(Log("AssetBrowserWindow created", "AssetBrowserWindow"));
  }

  AssetBrowserWindow::AssetBrowserWindow(
    AssetDirectoryPath                assetDirectoryPath,
    std::shared_ptr<IDwarfLogger>     logger,
    std::shared_ptr<ITextureFactory>  textureFactory,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IMaterialIO>      materialIO,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IAssetMetadata>   assetMetadata,
    std::shared_ptr<IMaterialCreator> materialCreator,
    std::shared_ptr<IShaderCreator>   shaderCreator,
    std::shared_ptr<IFileHandler>     fileHandler,
    std::shared_ptr<ISceneIO>         sceneIO,
    SerializedModule                  serializedModule)
    : IGuiModule(ModuleLabel("Asset Browser"),
                 ModuleType(MODULE_TYPE::ASSET_BROWSER),
                 ModuleID(std::make_shared<UUID>(
                   serializedModule.t["id"].get<std::string>())))
    , mAssetDirectoryPath(assetDirectoryPath)
    , mLogger(std::move(logger))
    , mCurrentDirectory(assetDirectoryPath)
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
  {
    mDirectoryHistory.push_back(mCurrentDirectory);
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
    if (mInputManager->GetMouseButtonDown(MOUSE_BUTTON::MOUSE_BUTTON_4))
    {
      GoBack();
    }
    else if (mInputManager->GetMouseButtonDown(MOUSE_BUTTON::MOUSE_BUTTON_5))
    {
      GoForward();
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

    if (firstFrame)
    {
      firstFrame = false;
      SetupDockspace(dockspaceId);
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
    for (const auto& directoryEntry :
         std::filesystem::directory_iterator(directory))
    {
      if (directoryEntry.is_directory())
      {
        if (ImGui::CollapsingHeader(
              directoryEntry.path().stem().string().c_str()))
        {
          ImGui::Indent(8.0F);
          RenderDirectoryLevel(directoryEntry.path());
          ImGui::Unindent(8.0F);
        }

        if (ImGui::IsItemClicked())
        {
          mCurrentDirectory = directoryEntry.path();
        }
      }
    }
  }

  void
  AssetBrowserWindow::RenderFolderStructure()
  {
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_NoMove;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(150, 150));
    ImGui::Begin("FolderStructure", nullptr, windowFlags);
    if (ImGui::CollapsingHeader("Assets"))
    {
      if (ImGui::IsItemClicked())
      {
        mCurrentDirectory = mAssetDirectoryPath.t;
      }
      ImGui::Indent(8.0F);
      RenderDirectoryLevel(mAssetDirectoryPath.t);
      ImGui::Unindent(8.0F);
    }
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
    ImGui::SliderFloat("Size", &mIconScale, 1.0F, 2.0F);
    ImGui::End();
    ImGui::PopStyleVar(2);
  }

  void
  AssetBrowserWindow::RenderFolderContent()
  {
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;
    windowFlags |= ImGuiWindowFlags_NoMove;

    ImGui::Begin("FolderContent", nullptr, windowFlags);

    ImGui::Dummy(ImGui::GetContentRegionAvail());

    if (ImGui::BeginPopupContextItem(
          "Backdrop")) // <-- use last item id as popup id
    {
      if (ImGui::BeginMenu("New"))
      {
        if (ImGui::BeginMenu("Material"))
        {
          if (ImGui::MenuItem("Pbr"))
          {
            mMaterialCreator->CreateMaterialAsset(MaterialType::PbrMaterial,
                                                  mCurrentDirectory);
          }
          if (ImGui::MenuItem("Unlit"))
          {
            mMaterialCreator->CreateMaterialAsset(MaterialType::UnlitMaterial,
                                                  mCurrentDirectory);
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
                                                mCurrentDirectory);
            }

            if (ImGui::MenuItem("Unlit"))
            {
              mShaderCreator->CreateShaderAsset(ShaderType::VertexType,
                                                ShaderSource::UnlitSource,
                                                mCurrentDirectory);
            }
            ImGui::EndMenu();
          }

          if (ImGui::BeginMenu("Fragment"))
          {
            if (ImGui::MenuItem("Pbr"))
            {
              mShaderCreator->CreateShaderAsset(ShaderType::FragmentType,
                                                ShaderSource::PbrSource,
                                                mCurrentDirectory);
            }

            if (ImGui::MenuItem("Unlit"))
            {
              mShaderCreator->CreateShaderAsset(ShaderType::FragmentType,
                                                ShaderSource::UnlitSource,
                                                mCurrentDirectory);
            }
            ImGui::EndMenu();
          }

          ImGui::EndMenu();
        }

        if (ImGui::MenuItem("Scene"))
        {
          mSceneIo->NewSceneAsset(mCurrentDirectory);
        }

        ImGui::EndMenu();
      }

      if (ImGui::MenuItem("Create Folder"))
      {
        mFileHandler->CreateDirectoryAt(mCurrentDirectory / "New Folder");
      }

      if (ImGui::MenuItem("Paste"))
      {
        if (std::filesystem::exists(mCopyPathBuffer))
        {
          mFileHandler->Copy(mCopyPathBuffer, mCurrentDirectory);
        }
      }

      if (ImGui::MenuItem("Open in file browser"))
      {
        mFileHandler->OpenPathInFileBrowser(mCurrentDirectory);
      }
      ImGui::EndPopup();
    }

    float column = 0;
    float rowOffset = 0;
    float tallestCell = 0;

    std::vector<std::filesystem::directory_entry> directories;
    std::vector<std::filesystem::directory_entry> files;

    // directories.
    for (const auto& directoryEntry :
         std::filesystem::directory_iterator(mCurrentDirectory))
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
        std::filesystem::relative(path, mAssetDirectoryPath.t);

      if (!(directoryEntry.path().has_extension() &&
            directoryEntry.path().extension() ==
              IAssetMetadata::METADATA_EXTENSION))
      {
        float padding = 16.0F * mIconScale;
        float halfPadding = padding / 2.0F;
        float cellWidth = 64.0F * mIconScale;
        float textWidth = cellWidth - padding;
        float textHeight =
          ImGui::CalcTextSize(
            relativePath.stem().string().c_str(), nullptr, false, textWidth)
            .y;
        float cellHeight = cellWidth + textHeight + halfPadding;

        tallestCell = std::max(cellHeight, tallestCell);

        ImVec2 cellMin = { ImGui::GetWindowContentRegionMin().x +
                             (column * (cellWidth + halfPadding)),
                           ImGui::GetWindowContentRegionMin().y + rowOffset };
        ImGui::SetCursorPos(cellMin);

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0F);
        if (mSelectedAsset == path)
        {
          ImGui::PushStyleColor(ImGuiCol_Button,
                                ImVec4(1.0F, 1.0F, 1.0F, 0.2F));
        }
        else
        {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        }
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4(1.0F, 1.0F, 1.0F, 0.2F));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4(1.0F, 1.0F, 1.0F, 0.4F));
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
            if (!directoryEntry.is_directory() && mAssetDatabase->Exists(path))
            {
              mEditorSelection->SelectAsset(mAssetDatabase->Retrieve(path));
            }
          }

          // TODO: Drag Asset
        }
        else
        {
          if (mSelectedAsset == directoryEntry.path() &&
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
            mCopyPathBuffer = path;
          }
          else if (ImGui::MenuItem("Duplicate"))
          {
            mFileHandler->Duplicate(path);
          }
          else if (ImGui::MenuItem("Rename"))
          {
            mRenamePathBuffer = path;
            SetRenameBuffer(path);
            mOpenRename = true;
          }
          if (ImGui::MenuItem("Open in file browser"))
          {
            if (directoryEntry.is_directory())
            {
              mFileHandler->OpenPathInFileBrowser(directoryEntry.path());
            }
            else
            {
              mFileHandler->OpenPathInFileBrowser(mCurrentDirectory);
            }
          }
          else if (ImGui::MenuItem("Delete"))
          {
            // TODO: Confirmation modal
            if (mAssetDatabase->Exists(path))
            {
              mAssetDatabase->Remove(path);
              mAssetMetadata->RemoveMetadata(path);
            }
            mFileHandler->Delete(path);
          }
          ImGui::EndPopup();
        }

        ImGui::SetCursorPos(
          ImVec2(cellMin.x + halfPadding, cellMin.y + halfPadding));

        static ImTextureID texID;
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
              texID =
                (ImTextureID)mTessellationControlShaderIcon->GetTextureID();
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
            else
            {
              texID = (ImTextureID)mUnknownFileIcon->GetTextureID();
            }
          }
          else
          {
            texID = (ImTextureID)mUnknownFileIcon->GetTextureID();
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

    if (mOpenRename)
    {
      ImGui::OpenPopup("rename_popup");
      mOpenRename = false;
    }

    if (ImGui::BeginPopup("rename_popup"))
    {
      std::filesystem::path old = mRenamePathBuffer;

      ImGui::InputText("##RenameInput",
                       mRenameBuffer.data(),
                       mRenameBuffer.capacity() + 1,
                       ImGuiInputTextFlags_CallbackResize,
                       DwarfUI::InputTextCallback,
                       &mRenameBuffer);
      if (ImGui::Button("Rename##RenameButton") && (mRenameBuffer.size() > 0))
      {
        std::filesystem::path newPath =
          mRenamePathBuffer.remove_filename().concat(mRenameBuffer);
        if (mAssetDatabase->Exists(old))
        {
          mAssetDatabase->Rename(old, newPath);
        }
        else
        {
          mAssetDatabase->RenameDirectory(old, newPath);
        }
        mFileHandler->Rename(old, newPath);
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
    if (mHistoryPos > 0)
    {
      mHistoryPos--;
      mCurrentDirectory = mDirectoryHistory[mHistoryPos];
    }
  }

  void
  AssetBrowserWindow::GoForward()
  {
    if (mHistoryPos < (mDirectoryHistory.size() - 1))
    {
      mHistoryPos++;
      mCurrentDirectory = mDirectoryHistory[mHistoryPos];
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
      // mModel->SetScene(SceneUtilities::LoadScene(directoryEntry.path()));
      // TODO: Use SceneIO
    }
    else
    {
      // TODO:s Open file
      mFileHandler->LaunchFile(directoryEntry.path());
    }
  }

  void
  AssetBrowserWindow::EnterDirectory(std::filesystem::path const& path)
  {
    mCurrentDirectory /= path.filename();
    if (mHistoryPos < (mDirectoryHistory.size() - 1))
    {
      for (int i = 0; i < (mDirectoryHistory.size() - mHistoryPos); i++)
      {
        mDirectoryHistory.pop_back();
      }
    }
    mDirectoryHistory.push_back(mCurrentDirectory);
    mHistoryPos = (int)mDirectoryHistory.size() - 1;
  }

  void
  AssetBrowserWindow::HandleShortcuts() const
  {
    // TODO: Implement shortcut handling
  }

  void
  AssetBrowserWindow::ClearSelection()
  {
    mSelectedAsset = "";
    mEditorSelection->ClearAssetSelection();
  }

  void
  AssetBrowserWindow::Deserialize(nlohmann::json moduleData)
  {
    mCurrentDirectory =
      (std::filesystem::path)moduleData["openedPath"].get<std::string>();
  }

  auto
  AssetBrowserWindow::Serialize() -> nlohmann::json
  {
    nlohmann::json serializedModule;

    serializedModule["openedPath"] = mCurrentDirectory;
    serializedModule["id"] = GetUuid()->toString();
    serializedModule["type"] = static_cast<int>(GetModuleType());
    serializedModule["label"] = GetModuleName();

    return serializedModule;
  }

  void
  AssetBrowserWindow::SetRenameBuffer(std::filesystem::path const& path)
  {
    if (mFileHandler->FileExists(path))
    {
      // TODO: test this
#ifdef _MSC_VER
      mRenameBuffer = mRenamePathBuffer.filename().string();
#else
      mRenameBuffer = mRenamePathBuffer.filename().string();
#endif
    }
    else if (mFileHandler->DirectoryExists(path))
    {
      // TODO: test this
#ifdef _MSC_VER
      mRenameBuffer = mRenamePathBuffer.filename().string();
#else
      mRenameBuffer = mRenamePathBuffer.filename().string();
#endif
    }
  }
}