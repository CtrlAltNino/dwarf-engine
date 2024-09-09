#include "AssetBrowserWindowFactory.h"
#include "Core/Asset/Database/IAssetDatabase.h"
#include "Editor/Modules/AssetBrowser/AssetBrowserWindow.h"
#include <memory>

namespace Dwarf
{
  AssetBrowserWindowFactory::AssetBrowserWindowFactory(
    AssetDirectoryPath                assetDirectoryPath,
    std::shared_ptr<ITextureFactory>  textureFactory,
    std::shared_ptr<IAssetDatabase>   assetDatabase,
    std::shared_ptr<IInputManager>    inputManager,
    std::shared_ptr<IEditorSelection> editorSelection,
    std::shared_ptr<IMaterialIO>      materialIO,
    std::shared_ptr<IMaterialFactory> materialFactory,
    std::shared_ptr<IAssetMetadata>   assetMetadata,
    std::shared_ptr<IMaterialCreator> materialCreator)
    : m_AssetDirectoryPath(assetDirectoryPath)
    , m_TextureFactory(textureFactory)
    , m_AssetDatabase(assetDatabase)
    , m_InputManager(inputManager)
    , m_EditorSelection(editorSelection)
    , m_MaterialIO(materialIO)
    , m_MaterialFactory(materialFactory)
    , m_AssetMetadata(assetMetadata)
    , m_MaterialCreator(materialCreator)
  {
  }

  std::unique_ptr<AssetBrowserWindow>
  AssetBrowserWindowFactory::Create() const
  {
    return std::make_unique<AssetBrowserWindow>(m_AssetDirectoryPath,
                                                m_TextureFactory,
                                                m_AssetDatabase,
                                                m_InputManager,
                                                m_EditorSelection,
                                                m_MaterialIO,
                                                m_MaterialFactory,
                                                m_AssetMetadata,
                                                m_MaterialCreator);
  }

  std::unique_ptr<AssetBrowserWindow>
  AssetBrowserWindowFactory::Create(SerializedModule serializedModule) const
  {
    return std::make_unique<AssetBrowserWindow>(m_AssetDirectoryPath,
                                                m_TextureFactory,
                                                m_AssetDatabase,
                                                m_InputManager,
                                                m_EditorSelection,
                                                m_MaterialIO,
                                                m_MaterialFactory,
                                                m_AssetMetadata,
                                                m_MaterialCreator,
                                                serializedModule);
  }
} // namespace Dwarf